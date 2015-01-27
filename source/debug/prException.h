// File: prException.h
/**
 * Copyright 2014 Paul Michael McNab
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


#ifndef __PREXCEPTION_H
#define __PREXCEPTION_H


#include <string>


// Class: prException
//      The engine exception base class.
class prException
{
public:
    // Method: prException
    //      Default constructor.
    prException();

    // Method: prException
    //      Constructor.
    //
    // Parameters:
    //      error - A descriptive error message
    prException(std::string error);

    // Method: prException
    //      Constructor.
    //
    // Parameters:
    //      error - A descriptive error message
    //      func  - The function that the exception occurred in
    //      file  - The file that the exception occurred in
    //      line  - The line number that the exception occurred in
    prException(std::string error, std::string func, std::string file, int line);

    // Method: ~prException
    //      Destructor.
    virtual ~prException() {}

    // Method: GetLine
    //      Gets the line number of the exception.
    int GetLine() const { return m_line; }

    // Method: GetError
    //      Gets the cause of the exception.
    const std::string GetError() const { return m_error; }

    // Method: GetFile
    //      Gets the file name where the exception occurred.
    const std::string GetFile() const { return m_filename; }

    // Method: GetFunction
    //      Gets the function/method name where the exception occurred.
    const std::string GetFunction() const { return m_function; }


protected:
    int         m_line;           // line number of the source file containing the error.
    std::string	m_error;          // The error causing the exception.
    std::string	m_filename;       // Filename of source file containing error.
    std::string	m_function;       // Function name containing error.
};


#endif//__PREXCEPTION_H
