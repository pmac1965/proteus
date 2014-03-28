// File: prAnalyticsBase.h
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


#ifndef __PRANALYTICSBASE_H
#define __PRANALYTICSBASE_H


// Class: prAnalyticsBase
//      The base class for analytics providers.
class prAnalyticsBase
{
public:
    // Method: prAnalyticsBase
    //      Ctor
    prAnalyticsBase();

    // Method: ~prAnalyticsBase
    //      Dtor
    virtual ~prAnalyticsBase();

    // Method: Construct
    //      Construct the provider.
    virtual void Construct(const char *appId, const char *secret);

    // Method: Submit
    //      Submits to the provider.
    virtual bool Submit(const char *name, const char *value) = 0;

    // Method: Commit
    //      Commits the submitted key/values to the provider.
    virtual void Commit();
};


#endif//__PRANALYTICSBASE_H
