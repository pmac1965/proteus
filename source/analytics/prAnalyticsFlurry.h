// File: prAnalyticsFlurry.h
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


#pragma once


#include "prAnalyticsBase.h"


// Class: prAnalyticsFlurry
//      Flurry privider
class prAnalyticsFlurry : public prAnalyticsBase
{
public:
    // Method: prAnalyticsFlurry
    //      Ctor
    prAnalyticsFlurry();

    // Method: ~prAnalyticsFlurry
    //      Dtor
    ~prAnalyticsFlurry();

    // Method: Construct
    //      Construct the provider.
    void Construct(const char *appId, const char *secret) override;

    // Method: Submit
    //      Submits to the provider.
    bool Submit(const char *name, const char *value) override;

    // Method: Commit
    //      Commits the submitted key/values to the provider.
    void Commit() override;
};
