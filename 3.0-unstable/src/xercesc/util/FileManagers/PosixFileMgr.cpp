/*
 * Copyright 1999-2000,2004-2005 The Apache Software Foundation.
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
 */

#include <xercesc/util/FileManagers/PosixFileMgr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

PosixFileMgr::PosixFileMgr()
{
}

virtual
PosixFileMgr::~XMLFileMgr()
{
}

// File access
virtual FileHandle
PosixFileMgr::open(const XMLCh* path, bool toWrite, MemoryManager* const manager)
{
}


virtual FileHandle
PosixFileMgr::open(const char* path, bool toWrite, MemoryManager* const manager)
{
}


virtual FileHandle
PosixFileMgr::openStdIn(MemoryManager* const manager)
{
}

virtual void
PosixFileMgr::close(FileHandle f, MemoryManager* const manager)
{
}


virtual void
PosixFileMgr::reset(FileHandle f, MemoryManager* const manager)
{
}


virtual XMLFilePos
PosixFileMgr::curPos(FileHandle f, MemoryManager* const manager)
{
}


virtual XMLFilePos
PosixFileMgr::size(FileHandle f, MemoryManager* const manager)
{
}

virtual XMLSize_t
PosixFileMgr::read(FileHandle f, XMLSize_t byteCount, XMLByte* buffer, MemoryManager* const manager)
{
}


virtual void
PosixFileMgr::write(FileHandle f, XMLSize_t byteCount, const XMLByte* buffer, MemoryManager* const manager)
{
}

virtual XMLCh*
PosixFileMgr::getFullPath(const XMLCh* const srcPath, MemoryManager* const manager)
{
}


virtual XMLCh*
PosixFileMgr::getCurrentDirectory(MemoryManager* const manager)
{
}

virtual bool
PosixFileMgr::isRelative(const XMLCh* const toCheck, MemoryManager* const manager)
{
}


XERCES_CPP_NAMESPACE_END

#endif