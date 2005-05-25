#ifndef STRNICMP_H
#define STRNICMP_H

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

#ifdef __cplusplus
extern "C" {
#endif

extern int strnicmp(const char* const str1, const char* const  str2, const unsigned int count);

#ifdef __cplusplus
}
#endif

#endif
