// File: prDialogListener.h
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


#ifndef __PRDIALOGLISTENER_H
#define __PRDIALOGLISTENER_H


// Namespaces
namespace Proteus {
namespace Gui {


// Class: prDialogListener
//      The listener for a dialog
class prDialogListener
{
public:
    // Method: OnDlgButtonPressed
    //      A method which the handling class needs to implement
    //
    // Parameters:
    //      name - The name of the button pressed
    virtual bool OnDlgButtonPressed(const char *name, Proteus::Core::s32 index) = 0;

    // Method: OnDlgButtonReleased
    //      A method which the handling class needs to implement
    //
    // Parameters:
    //      name - The name of the button released
    virtual bool OnDlgButtonReleased(const char *name, Proteus::Core::s32 index) = 0;
};


}}// Namespaces


#endif//__PRDIALOGLISTENER_H
