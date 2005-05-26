/*
 * Copyright 1999-2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <config.h>

#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/internal/XMLReader.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/DefaultPanicHandler.hpp>
#include <xercesc/util/XMLInitializer.hpp>
#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/internal/MemoryManagerArrayImpl.hpp>

#include <xercesc/util/XMLFileMgr.hpp>
#if XERCES_USE_FILEMGR_POSIX
#	include <xercesc/util/FileManagers/PosixFileMgr.hpp>
#endif

#include <xercesc/util/XMLMutexMgr.hpp>
#if XERCES_USE_MUTEXMGR_POSIX
#	include <xercesc/util/MutexManagers/PosixMutexMgr.hpp>
#endif

#include <xercesc/util/XMLAtomicOpMgr.hpp>
#if XERCES_USE_ATOMICOPMGR_POSIX
#	include <xercesc/util/AtomicOpManagers/PosixAtomicOpMgr.hpp>
#endif

#include <limits.h>

// *** TODO: protect and x-platform
#include <sys/timeb.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local data members
//
//  gSyncMutex
//      This is a mutex that will be used to synchronize access to some of
//      the static data of the platform utilities class and here locally.
// ---------------------------------------------------------------------------
static XMLMutex*                gSyncMutex = 0;
static long                     gInitFlag = 0;

// ---------------------------------------------------------------------------
//  Global data
//
//	gXMLCleanupList
//		This is a list of cleanup functions to be called on
//		XMLPlatformUtils::Terminate.  Their function is to reset static
//		data in classes that use it.
//
//	gXMLCleanupListMutex
//		This is a mutex that will be used to synchronise access to the global
//		static data cleanup list
// ---------------------------------------------------------------------------
XMLRegisterCleanup*	gXMLCleanupList = 0;
XMLMutex*           gXMLCleanupListMutex = 0;


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Static Data Members
// ---------------------------------------------------------------------------
XMLNetAccessor*         XMLPlatformUtils::fgNetAccessor = 0;
XMLTransService*        XMLPlatformUtils::fgTransService = 0;
PanicHandler*           XMLPlatformUtils::fgUserPanicHandler = 0;
PanicHandler*           XMLPlatformUtils::fgDefaultPanicHandler = 0;
MemoryManager*          XMLPlatformUtils::fgMemoryManager = 0;
MemoryManagerArrayImpl  gArrayMemoryManager;
MemoryManager*          XMLPlatformUtils::fgArrayMemoryManager = &gArrayMemoryManager;
bool                    XMLPlatformUtils::fgMemMgrAdopted = true;

XMLFileMgr*             XMLPlatformUtils::fgFileMgr = 0;
XMLMutexMgr*            XMLPlatformUtils::fgMutexMgr = 0;
XMLAtomicOpMgr*         XMLPlatformUtils::fgAtomicOpMgr = 0;

XMLMutex*				XMLPlatformUtils::fgAtomicMutex = 0;

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Init/term methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::Initialize(const char*          const locale 
                                , const char*          const nlsHome
                                ,       PanicHandler*  const panicHandler
                                ,       MemoryManager* const memoryManager
                                ,       bool                 toInitStatics)
{
    //
    //  Effects of overflow:
    //  . resouce re-allocations
    //  . consequently resource leaks
    //  . potentially terminate() may never get executed
    //
    //  We got to prevent overflow from happening.
    //  no error or exception
    //
    if (gInitFlag == LONG_MAX)
        return;
	
    //
    //  Make sure we haven't already been initialized. Note that this is not
    //  thread safe and is not intended for that. Its more for those COM
    //  like processes that cannot keep up with whether they have initialized
    //  us yet or not.
    //
    gInitFlag++;

    if (gInitFlag > 1)
      return;

    // Set pluggable memory manager
    if (!fgMemoryManager)
    {
        if (memoryManager)
        {
            fgMemoryManager = memoryManager;
            fgMemMgrAdopted = false;
        }
        else
        {
            fgMemoryManager = new MemoryManagerImpl();
        }
    }

    /***
     * Panic Handler:
     *
     ***/
    if (!panicHandler)
    {
        fgDefaultPanicHandler = new DefaultPanicHandler();
    }
    else
    {
        fgUserPanicHandler = panicHandler;
    }
    
    
    // Initialize the platform-specific mutex file, and atomic op mgrs
    fgMutexMgr		= makeMutexMgr(fgMemoryManager);
    fgAtomicOpMgr	= makeAtomicOpMgr(fgMemoryManager);
    fgFileMgr		= makeFileMgr(fgMemoryManager);
    
    
    // Create the local sync mutex
    gSyncMutex		= new XMLMutex;
    fgAtomicMutex	= new XMLMutex;


	// Create the mutex for the static data cleanup list
    gXMLCleanupListMutex = new XMLMutex;

    //
    //  Ask the per-platform code to make the desired transcoding service for
    //  us to use. This call cannot throw any exceptions or do anything that
    //  cause any transcoding to happen. It should create the service and
    //  return it or zero if it cannot.
    //
    //  This one also cannot use any utility services. It can only create a
    //  transcoding service object and return it.
    //
    //  If we cannot make one, then we call panic to end the process.
    //
    fgTransService = makeTransService();

    if (!fgTransService)
        panic(PanicHandler::Panic_NoTransService);

    // Initialize the transcoder service
    fgTransService->initTransService();

    //
    //  Try to create a default local code page transcoder. This is the one
    //  that will be used internally by the XMLString class. If we cannot
    //  create one, then call the panic method.
    //
    XMLLCPTranscoder* defXCode = XMLPlatformUtils::fgTransService->makeNewLCPTranscoder();
    if (!defXCode)
        panic(PanicHandler::Panic_NoDefTranscoder);
    XMLString::initString(defXCode, fgMemoryManager);

    //
    //  Now lets ask the per-platform code to give us an instance of the type
    //  of network access implementation he wants to use. This can return
    //  a zero pointer if this platform doesn't want to support this.
    //
    fgNetAccessor = makeNetAccessor();

    /***
     * Message Loader:
     *
     *     Locale setting 
     *     nlsHome setting 
     ***/
    XMLMsgLoader::setLocale(locale);
    XMLMsgLoader::setNLSHome(nlsHome);

    if (toInitStatics) {
        XMLInitializer::InitializeAllStaticData();
    }
}


void XMLPlatformUtils::Terminate()
{
    //
    // To prevent it from running underflow.
    // otherwise we come to delete non-existing resources.
    //
    //  no error or exception
    //
    if (gInitFlag == 0)
        return;

	gInitFlag--;
	
	if (gInitFlag > 0)
		return;

    // Delete any net accessor that got installed
    delete fgNetAccessor;
    fgNetAccessor = 0;

    //
    //  Call some other internal modules to give them a chance to clean up.
    //  Do the string class last in case something tries to use it during
    //  cleanup.
    //
    XMLString::termString();

    // Clean up the the transcoding service
    delete fgTransService;
    fgTransService = 0;

    // Clean up mutexes
    delete gSyncMutex;		gSyncMutex = 0;
    delete fgAtomicMutex;	fgAtomicMutex = 0;

	// Clean up statically allocated, lazily cleaned data in each class
	// that has registered for it.
	// Note that calling doCleanup() also unregisters the cleanup
	// function, so that we are chewing the list down to nothing here
	while (gXMLCleanupList)
		gXMLCleanupList->doCleanup();

	// Clean up the mutex for accessing gXMLCleanupList
	delete gXMLCleanupListMutex;
	gXMLCleanupListMutex = 0;

	// Clean up our mgrs
	delete fgFileMgr;		fgFileMgr = 0;
	delete fgAtomicOpMgr;	fgAtomicOpMgr = 0;
	delete fgMutexMgr;		fgMutexMgr = 0;
	
    /***
     *  de-allocate resource
     *
     *  refer to discussion in the Initialize()
     ***/
    XMLMsgLoader::setLocale(0);
    XMLMsgLoader::setNLSHome(0);

    delete fgDefaultPanicHandler;
    fgDefaultPanicHandler = 0;
    fgUserPanicHandler = 0;

    // de-allocate default memory manager
    if (fgMemMgrAdopted)
        delete fgMemoryManager;
    else
        fgMemMgrAdopted = true;

    // set memory manager to 0
    fgMemoryManager = 0;

    // And say we are no longer initialized
    gInitFlag = 0;
}




// ---------------------------------------------------------------------------
//  XMLPlatformUtils: The panic method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::panic(const PanicHandler::PanicReasons reason)
{
    fgUserPanicHandler? fgUserPanicHandler->panic(reason) : fgDefaultPanicHandler->panic(reason);	
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
XMLFileMgr*
XMLPlatformUtils::makeFileMgr(MemoryManager* const memmgr)
{
	XMLFileMgr* mgr = NULL;
	
	#if XERCES_USE_FILEMGR_POSIX
		mgr = new (memmgr) PosixFileMgr;
	#else
		#error No File Manager configured for platform! You must configure it.
	#endif
	
	return mgr;
}


FileHandle
XMLPlatformUtils::openFile(const char* const fileName
                           , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->open(fileName, false, memmgr);
}


FileHandle
XMLPlatformUtils::openFile(const XMLCh* const fileName, MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->open(fileName, false, memmgr);
}


FileHandle
XMLPlatformUtils::openFileToWrite(const char* const fileName
                                  , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->open(fileName, true, memmgr);
}


FileHandle
XMLPlatformUtils::openFileToWrite(const XMLCh* const fileName
                                  , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

 	return fgFileMgr->open(fileName, true, memmgr);
}


FileHandle
XMLPlatformUtils::openStdInHandle(MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->openStdIn(memmgr);
}


void
XMLPlatformUtils::closeFile(const FileHandle theFile
                            , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->close(theFile, memmgr);
}

void
XMLPlatformUtils::resetFile(FileHandle theFile
                            , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->reset(theFile, memmgr);
}


XMLFilePos
XMLPlatformUtils::curFilePos(const FileHandle theFile
                             , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->curPos(theFile, memmgr);
}

XMLFilePos
XMLPlatformUtils::fileSize(const FileHandle theFile
                           , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->size(theFile, memmgr);
}


XMLSize_t
XMLPlatformUtils::readFileBuffer(   const FileHandle      theFile
                                 ,  const XMLSize_t		  toRead
                                 ,        XMLByte* const  toFill
                                 ,  MemoryManager* const  memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

    return fgFileMgr->read(theFile, toRead, toFill, memmgr);
}


void
XMLPlatformUtils::writeBufferToFile(   const   FileHandle   theFile
                                    ,  const XMLSize_t		toWrite
                                    ,  const XMLByte* const toFlush
                                    ,  MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	fgFileMgr->write(theFile, toWrite, toFlush, memmgr);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getFullPath(const XMLCh* const srcPath,
                                     MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

	return fgFileMgr->getFullPath(srcPath, memmgr);
}


XMLCh* XMLPlatformUtils::getCurrentDirectory(MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

    return fgFileMgr->getCurrentDirectory(memmgr);
}


bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck
                                  , MemoryManager* const memmgr)
{
    if (!fgFileMgr)
		ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero, memmgr);

    return fgFileMgr->isRelative(toCheck, memmgr);
}


/*
	TODO: we probably want to move this as an inline function into the weavepaths code.
*/
bool XMLPlatformUtils::isAnySlash(XMLCh c) 
{
    return (
			false
		 || chForwardSlash == c
#if TODO_AUTOCONF_CHECK
		 || chBackSlash == c
#endif
     );
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------
unsigned long XMLPlatformUtils::getCurrentMillis()
{
	// *** TODO: per platform support here
    timeb aTime;
    ftime(&aTime);
    return (unsigned long)(aTime.time*1000 + aTime.millitm);

}


// -----------------------------------------------------------------------
//  Mutex methods
// -----------------------------------------------------------------------
XMLMutexMgr* XMLPlatformUtils::makeMutexMgr(MemoryManager* const memmgr)
{
	XMLMutexMgr* mgr = NULL;
	
	#if XERCES_USE_MUTEXMGR_POSIX
		mgr = new (memmgr) PosixMutexMgr;
	#else
		#error No Mutex Manager configured for platform! You must configure it.
	#endif
	
	return mgr;
}


XMLMutexHandle XMLPlatformUtils::makeMutex(MemoryManager* const memmgr)
{
    if (!fgMutexMgr)
		XMLPlatformUtils::panic(PanicHandler::Panic_MutexErr);

	return fgMutexMgr->create(memmgr);
}


void XMLPlatformUtils::closeMutex(XMLMutexHandle const mtx, MemoryManager* const memmgr)
{
    if (!fgMutexMgr)
		XMLPlatformUtils::panic(PanicHandler::Panic_MutexErr);

	fgMutexMgr->destroy(mtx, memmgr);
}


void XMLPlatformUtils::lockMutex(XMLMutexHandle const mtx)
{
    if (!fgMutexMgr)
		XMLPlatformUtils::panic(PanicHandler::Panic_MutexErr);

	fgMutexMgr->lock(mtx);
}


void XMLPlatformUtils::unlockMutex(XMLMutexHandle const mtx)
{
    if (!fgMutexMgr)
		XMLPlatformUtils::panic(PanicHandler::Panic_MutexErr);

	fgMutexMgr->unlock(mtx);
}


// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------
XMLAtomicOpMgr* XMLPlatformUtils::makeAtomicOpMgr(MemoryManager* const memmgr)
{
	XMLAtomicOpMgr* mgr = NULL;
	
	#if XERCES_USE_ATOMICOPMGR_POSIX
		mgr = new (memmgr) PosixAtomicOpMgr;
	#else
		#error No AtomicOp Manager configured for platform! You must configure it.
	#endif
	
	return mgr;
}


void* XMLPlatformUtils::compareAndSwap(void**            toFill
                                     , const void* const newValue
                                     , const void* const toCompare)
{
    if (!fgAtomicOpMgr)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);

	return fgAtomicOpMgr->compareAndSwap(toFill, newValue, toCompare);
}


int XMLPlatformUtils::atomicIncrement(int &location)
{
    if (!fgAtomicOpMgr)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);

	return fgAtomicOpMgr->increment(location);
}


int XMLPlatformUtils::atomicDecrement(int &location)
{
    if (!fgAtomicOpMgr)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);

	return fgAtomicOpMgr->decrement(location);
}




// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Msg support methods
// ---------------------------------------------------------------------------
XMLMsgLoader* XMLPlatformUtils::loadMsgSet(const XMLCh* const msgDomain)
{
    //
    //  Ask the platform support to load up the correct type of message
    //  loader for the indicated message set. We don't check here whether it
    //  works or not. That's their decision.
    //
    return loadAMsgSet(msgDomain);
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: NEL Character Handling
// ---------------------------------------------------------------------------
void XMLPlatformUtils::recognizeNEL(bool state, MemoryManager* const manager) {

    //Make sure initialize has been called
    if (gInitFlag == 0) {
        return;
    }

    if (state) {

        if (!XMLChar1_0::isNELRecognized()) {
            XMLChar1_0::enableNELWS();
        }
    }
    else {

        if (XMLChar1_0::isNELRecognized()) {
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::NEL_RepeatedCalls, manager);
        }
    }
}


bool XMLPlatformUtils::isNELRecognized() {

    return XMLChar1_0::isNELRecognized();
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: IANA Encoding checking setting
// ---------------------------------------------------------------------------
void XMLPlatformUtils::strictIANAEncoding(const bool state) {

    //Make sure initialize has been called
    if (gInitFlag == 0) {
        return;
    }

    fgTransService->strictIANAEncoding(state);
}


bool XMLPlatformUtils::isStrictIANAEncoding() {

    if (gInitFlag)
        return fgTransService->isStrictIANAEncoding();

    return false;
}

XERCES_CPP_NAMESPACE_END
