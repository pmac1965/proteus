/**
 * prException.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "prException.h"


using namespace std;


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prException::prException() : m_line     (-1)
                           , m_error    ("Unknown error")
                           , m_filename ("Unknown file name")
                           , m_function ("Unknown function")
{
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prException::prException(string error) : m_line     (-1)
                                       , m_error    (error)
                                       , m_filename ("Unknown file name")
                                       , m_function ("Unknown function")
{
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prException::prException(string error, string func, string file, int line) : m_line     (line)
                                                                           , m_error    (error)
                                                                           , m_filename (file)
                                                                           , m_function (func)
{
}