/*
 * Copyright 2005 The Apache Software Foundation.
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
 * $Id: DOM.hpp,v 1.12 2004/09/08 13:55:38 peiyongz Exp $
 */

#include "strnicmp.h"
#include "config.h"

#if defined(HAVE_STRING)
#include <string.h>
#endif

int strnicmp(const char* const str1, const char* const  str2, const unsigned int count)
{
#if defined(HAVE_STRNCASECMP)
	return (count == 0) ? 0 : strncasecmp( str1, str2, count);
#else
	#error Need implementation of strnicmp compatibility function
#endif
}
