/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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


#if !defined(XERCESDEFS_HPP)
#define XERCESDEFS_HPP

// ---------------------------------------------------------------------------
//  The file xerces_hdr_config defines critical configuration information
//	used by the remainder of this file.
//
//	There are two major configuration files:
//		- xerces_autoconf_config.h	-- Contains defines that are safe for
//									   access through public headers.
//
//		- config.h					-- Contains defines that may conflict
//									   with other packages; should only be
//									   included by Xerces implementation files.
//
//	Both of these files are generated through the autoconf/configure process.
// ---------------------------------------------------------------------------

//
// If this is an autoconf configured build, we include Xerces_autoconf_config.
// Otherwise we include a preconfigured config appropriate for the particular
// platform.
//
#if FLAG_TO_INDICATE_WINDOWS_BUILD
#	include <xercesc/util/Xerces_windows_config.h>
#else
//  If the next line generates an error then you haven't run ./configure
#	include	<xercesc/util/Xerces_autoconf_config.h>
#endif

// ---------------------------------------------------------------------------
//  Include the Xerces version information; this is kept in a separate file to
//  make modification simple and obvious. Updates to the version header file
// ---------------------------------------------------------------------------
#include    <xercesc/util/XercesVersion.hpp>


// ---------------------------------------------------------------------------
//  Some general typedefs that are defined for internal flexibility.
//
//  Note  that UTF16Ch is fixed at 16 bits, whereas XMLCh floats in size per
//  platform, to whatever is the native wide char format there. UCS4Ch is
//  fixed at 32 bits. The types we defined them in terms of are defined per
//  compiler, using whatever types are the right ones for them to get these
//  16/32 bit sizes.
//
// ---------------------------------------------------------------------------
typedef unsigned char       XMLByte;
typedef XMLUInt16           UTF16Ch;
typedef XMLUInt32           UCS4Ch;


// ---------------------------------------------------------------------------
//  Handle boolean. If the platform can handle booleans itself, then we
//  map our boolean type to the native type. Otherwise we create a default
//  one as an int and define const values for true and false.
//
//  This flag will be set in the per-development environment stuff above.
// ---------------------------------------------------------------------------
#if defined(NO_NATIVE_BOOL)
  #ifndef bool
    typedef int     bool;
  #endif
  #ifndef true
    #define  true     1
  #endif
  #ifndef false
    #define false 0
  #endif
#endif

#if defined(XML_NETBSD)
#include       <xercesc/util/Platforms/NetBSD/NetBSDDefs.hpp>
#endif

// ---------------------------------------------------------------------------
//  According to whether the compiler suports L"" type strings, we define
//  the XMLStrL() macro one way or another.
// ---------------------------------------------------------------------------
#if defined(XML_LSTRSUPPORT)
#define XMLStrL(str)  L##str
#else
#define XMLStrL(str)  str
#endif


// ---------------------------------------------------------------------------
// Define namespace symbols if the compiler supports it.
// ---------------------------------------------------------------------------
#if defined(XERCES_HAS_CPP_NAMESPACE)
    #define XERCES_CPP_NAMESPACE_BEGIN namespace XERCES_CPP_NAMESPACE {
    #define XERCES_CPP_NAMESPACE_END  }
    #define XERCES_CPP_NAMESPACE_USE using namespace XERCES_CPP_NAMESPACE;
    #define XERCES_CPP_NAMESPACE_QUALIFIER XERCES_CPP_NAMESPACE::

    namespace XERCES_CPP_NAMESPACE { }
    namespace xercesc = XERCES_CPP_NAMESPACE;
#else
    #define XERCES_CPP_NAMESPACE_BEGIN
    #define XERCES_CPP_NAMESPACE_END
    #define XERCES_CPP_NAMESPACE_USE
    #define XERCES_CPP_NAMESPACE_QUALIFIER
#endif

#if defined(XERCES_STD_NAMESPACE)
	#define XERCES_USING_STD(NAME) using std :: NAME;
	#define XERCES_STD_QUALIFIER  std ::
#else
	#define XERCES_USING_STD(NAME)
	#define XERCES_STD_QUALIFIER 
#endif


// ---------------------------------------------------------------------------
//  Set up the import/export keyword  for our core projects. The
//  PLATFORM_XXXX keywords are set in the per-development environment
//  include above.
// ---------------------------------------------------------------------------
#if defined(PROJ_XMLUTIL)
#define XMLUTIL_EXPORT XERCES_PLATFORM_EXPORT
#else
#define XMLUTIL_EXPORT XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_XMLPARSER)
#define XMLPARSER_EXPORT XERCES_PLATFORM_EXPORT
#else
#define XMLPARSER_EXPORT XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_SAX4C)
#define SAX_EXPORT XERCES_PLATFORM_EXPORT
#else
#define SAX_EXPORT XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_SAX2)
#define SAX2_EXPORT XERCES_PLATFORM_EXPORT
#else
#define SAX2_EXPORT XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_DOM)
#define CDOM_EXPORT XERCES_PLATFORM_EXPORT
#else
#define CDOM_EXPORT XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_DEPRECATED_DOM)
#define DEPRECATED_DOM_EXPORT XERCES_PLATFORM_EXPORT
#else
#define DEPRECATED_DOM_EXPORT XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_PARSERS)
#define PARSERS_EXPORT  XERCES_PLATFORM_EXPORT
#else
#define PARSERS_EXPORT  XERCES_PLATFORM_IMPORT
#endif

#if defined(PROJ_VALIDATORS)
#define VALIDATORS_EXPORT  XERCES_PLATFORM_EXPORT
#else
#define VALIDATORS_EXPORT  XERCES_PLATFORM_IMPORT
#endif

#endif
