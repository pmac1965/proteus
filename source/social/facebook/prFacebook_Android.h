// File: prFacebook_Android.h
/**
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


#ifndef __PRFACEBOOK_ANDROID_H
#define __PRFACEBOOK_ANDROID_H


#include "prFacebook.h"


// Class: prFacebook_Android
//      A base class for abstracting application specific facebook
//      functionality into a cross platform format
class prFacebook_Android : public prFacebook
{
public:
    // Method: prFacebook_Android
    //      Ctor
    prFacebook_Android();

    // Method: ~prFacebook_Android
    //      Ctor
    virtual ~prFacebook_Android();

    // Method: Initialise
    //      Initialise facebook for a specific platform
    bool Initialise();

    // Method: Update
    //      Update facebook for a specific platform
    bool Update();
};


#endif//__PRFACEBOOK_ANDROID_H
