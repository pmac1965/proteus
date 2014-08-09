/**
 * prAnalytics.cpp
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


#include "prAnalytics.h"
#include "prAnalyticsBase.h"
#include "../core/prMacros.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAnalytics::prAnalytics() : m_pProvider(NULL)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAnalytics::~prAnalytics()
{
    PRSAFE_DELETE(m_pProvider);
}


/// ---------------------------------------------------------------------------
/// Add an analytics provider.
/// ---------------------------------------------------------------------------
void prAnalytics::AddProvider(prAnalyticsBase *provider)
{
    if (provider)
    {
        if (m_pProvider)
        {
            PRWARN("Analytics provider has already been set.");
            return;
        }

        m_pProvider = provider;
    }
}


/// ---------------------------------------------------------------------------
/// Construct the provider.
/// ---------------------------------------------------------------------------
void prAnalytics::Construct(const char *uid, const char *secret)
{
    if (m_pProvider)
    {
        m_pProvider->Construct(uid, secret);
    }
}


/// ---------------------------------------------------------------------------
/// Submits to the privider.
/// ---------------------------------------------------------------------------
bool prAnalytics::Submit(const char *name, const char *values)
{
    bool result = false;
    
    if (m_pProvider)
    {
        result = m_pProvider->Submit(name, values);        
    }
    
    return result;
}


/// ---------------------------------------------------------------------------
/// Commits the submitted key/values to the provider.
/// ---------------------------------------------------------------------------
void prAnalytics::Commit()
{
}


