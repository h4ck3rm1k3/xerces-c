#
# Copyright 2005 The Apache Software Foundation.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# $Id: DOM.hpp,v 1.12 2004/09/08 13:55:38 peiyongz Exp $
#


check_PROGRAMS =					${testprogs}

LDADD =								../obj/libxerces.la
AM_CPPFLAGS =						-I${top_srcdir}/src

testprogs =

testprogs +=						_DOMTest
_DOMTest_SOURCES = 					DOM/DOMTest/DTest.cpp \
									DOM/DOMTest/DTest.h

testprogs +=						_DOMMemTest
_DOMMemTest_SOURCES = 				DOM/DOMMemTest/DOMMemTest.cpp

testprogs +=						_Normalizer
_Normalizer_SOURCES = 				DOM/Normalizer/Normalizer.cpp \
									DOM/Normalizer/Normalizer.hpp
TestNormalizer_EXTRA =				DOM/Normalizer/expectedOutput

testprogs +=						_RangeTest
_RangeTest_SOURCES = 				DOM/RangeTest/RangeTest.cpp

testprogs +=						_Traversal
_Traversal_SOURCES = 				DOM/Traversal/Traversal.cpp

testprogs +=						_TypeInfo
_TypeInfo_SOURCES =	 				DOM/TypeInfo/TypeInfo.cpp \
									DOM/TypeInfo/TypeInfo.hpp
_TypeInfo_EXTRA =					DOM/TypeInfo/data/combined.dtd \
									DOM/TypeInfo/data/combined.xml \
									DOM/TypeInfo/data/combined.xsd \
									DOM/TypeInfo/data/SecondSchema.xsd \
									DOM/TypeInfo/data/TypeInfo.dtd \
									DOM/TypeInfo/data/TypeInfo.xml \
									DOM/TypeInfo/data/TypeInfo.xsd \
									DOM/TypeInfo/data/TypeInfoJustDTD.xml \
									DOM/TypeInfo/data/TypeInfoNoDTD.xml \
									DOM/TypeInfo/data/TypeInfoNoDTD.xsd

testprogs +=						_EncodingTest
_EncodingTest_SOURCES = 			EncodingTest/EncodingTest.cpp

testprogs +=						_InitTermTest
_InitTermTest_SOURCES = 			InitTermTest/InitTermTest.cpp \
									InitTermTest/InitTermTest.hpp

# This tests the deprecated dom, which we don't build at the moment
#testprogs +=						_MemHandlerTest
#_MemHandlerTest_SOURCES = 			MemHandlerTest/MemoryMonitor.cpp \
#									MemHandlerTest/MemoryMonitor.hpp \
#									MemHandlerTest/SimpleHashPtr.cpp \
#									MemHandlerTest/SimpleHashPtr.hpp \
#									MemHandlerTest/SimpleValueHashTableOf.hpp

# Doesn't compile under gcc4 for some reason
# dcargill says this is obsolete and we can delete it.
#testprogs +=						_ParserTest
#_ParserTest_SOURCES = 				ParserTest/ParserTest.cpp \
#									ParserTest/ParserTest.hpp \
#									ParserTest/ParserTest_Parser.cpp \
#									ParserTest/ParserTest_Parser.hpp

# Needs to use generic pthread conditionals HAS_PTHREADS
#testprogs +=						_ThreadTest
#_ThreadTest_SOURCES = 				ThreadTest/ThreadTest.cpp

# Fails to compile under gcc 4 (ambiguous calls to NullPointerException)
# dcargill says this is obsolete and we can delete it.
#testprogs +=						_UtilTests
#_UtilTests_SOURCES = 				UtilTests/CoreTests_BitSet.cpp \
#									UtilTests/CoreTests_CountedPointer.cpp \
#									UtilTests/CoreTests_RefArray.cpp \
#									UtilTests/CoreTests_RefHashTable.cpp \
#									UtilTests/CoreTests_RefStack.cpp \
#									UtilTests/CoreTests_RefVector.cpp \
#									UtilTests/CoreTests_String.cpp \
#									UtilTests/CoreTests_Transcoders.cpp \
#									UtilTests/CoreTests_URL.cpp \
#									UtilTests/CoreTests_ValueArray.cpp \
#									UtilTests/CoreTests_ValueStack.cpp \
#									UtilTests/CoreTests_ValueVector.cpp \
#									UtilTests/CoreTestsMain.cpp \
#									UtilTests/CoreTests.hpp

testprogs +=						_XSerializerTest
_XSerializerTest_SOURCES = 			XSerializerTest/XSerializerHandlers.cpp \
									XSerializerTest/XSerializerHandlers.hpp \
									XSerializerTest/XSerializerTest.cpp \
									XSerializerTest/XSerializerTest.hpp

# Fails to compile under gcc4 (values too long for long type)
testprogs +=						_XSValueTest
_XSValueTest_SOURCES = 				XSValueTest/XSValueTest.cpp \
									XSValueTest/XSValueTest.hpp


