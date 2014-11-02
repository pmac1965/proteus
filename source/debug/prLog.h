// File: prLog.h
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


#ifndef __PRLOG_H
#define __PRLOG_H


// Function: prLog 
//      Outputs a debug string to an error log window.
//      The actual location is platform dependant, but will generally be the debuggers output window.
//
// Parameters:
//      message - The message to write
//
// Notes:
//      This type of logging is designed for error messages only as it
//      works in release mode as well as debug.
//
// Notes:
//      By default this type of logging is disabled.
//
// See Also:
//      <prLogEnable>
void prLog(const char *message);

// Method: prLogEnable
//      Enables logging.
//
// Notes:
//      This type of logging will always appear as it still
//      works in release mode.
//
// Notes:
//      By default this type of logging is disabled.
void prLogEnable(bool state);


#endif//__PRLOG_H
