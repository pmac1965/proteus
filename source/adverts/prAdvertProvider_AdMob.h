// File: prAdvertProvider_AdMob.h
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


#ifndef __PRADVERTPROVIDER_ADMOB_H
#define __PRADVERTPROVIDER_ADMOB_H


#include "prAdvertProvider.h"


// Class: prAdvertProvider_AdMob
//      Ad provider for AdMob
class prAdvertProvider_AdMob : public prAdvertProvider
{
public:
    // Method: prAdvertProvider_AdMob
    //      Ctor
    prAdvertProvider_AdMob();

    // Method: ~prAdvertProvider_AdMob
    //      Dtor
    ~prAdvertProvider_AdMob();

    // Method: Init
    //      Init advert provider.
    void Init();

    // Method: Show
    //      Show advert
    void Show(s32 type);

    // Method: Hide
    //      Hide advert.
    void Hide();

    // Method: Update
    //      Update the provider
    void Update();

    // Method: Draw
    //      Draw the provider
    void Draw();


private:

    bool    m_visible;
    bool    m_exp0;
    bool    m_exp1;
    bool    m_exp2;
};


#endif//__PRADVERTPROVIDER_ADMOB_H
