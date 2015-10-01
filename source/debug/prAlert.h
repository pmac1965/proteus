// File: prAlert.h
//      Contains cross platform error dialogs designed for game use only.
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


#pragma once


// Forward declarations
class prWindow;


// Method: prAlertShowError
//      Shows a cross platfrom error message dialog
//
// Parameters:
//      title   - The dialogs title text
//      message - The dialogs message text
//      pWindow - Optional window pointer
//
// Notes:
//      On a *PC* the optional window pointer is used to determine the dialogs parent window.
//
// Notes:
//      If a dialog is not available, then message will be logged to the console
void prAlertShowError(const char *title, const char *message, prWindow *pWindow = 0);


// Method: prAlertShowInformation
//      Shows a cross platfrom information message dialog
//
// Parameters:
//      title   - The dialogs title text
//      message - The dialogs message text
//      pWindow - Optional window pointer
//
// Notes:
//      On a *PC* the optional window pointer is used to determine the dialogs parent window.
//
// Notes:
//      If a dialog is not available, then message will be logged to the console
void prAlertShowInformation(const char *title, const char *message, prWindow *pWindow = 0);


// Method: prAlertShowWarning
//      Shows a cross platfrom warning message dialog
//
// Parameters:
//      title   - The dialogs title text
//      message - The dialogs message text
//      pWindow - Optional window pointer
//
// Notes:
//      On a *PC* the optional window pointer is used to determine the dialogs parent window.
//
// Notes:
//      If a dialog is not available, then message will be logged to the console
void prAlertShowWarning(const char *title, const char *message, prWindow *pWindow = 0);
