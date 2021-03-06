// File: prAnalytics.h
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


// Forward declarations
class prAnalyticsBase;


// Class: prAnalytics
//      A cross platform interface for analytics.
class prAnalytics
{
public:
    // Method: prAnalytics
    //      Ctor
    prAnalytics();

    // Method: ~prAnalytics
    //      Dtor
    ~prAnalytics();

    // Method: AddProvider
    //      Add an analytics provider.
    void AddProvider(prAnalyticsBase *provider);

    // Method: Construct
    //      Construct the provider.
    void Construct(const char *appId, const char *secret);

    // Method: Submit
    //      Submits to the provider.
    bool Submit(const char *name, const char *value);

    // Method: Commit
    //      Commits the submitted key/values to the provider.
    void Commit();


private:

    prAnalyticsBase *m_pProvider;
};
