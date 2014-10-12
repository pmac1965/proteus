// File: prSingleton.h
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


#ifndef __PRSINGLETON_H
#define __PRSINGLETON_H


#include "prMacros.h"
#include "../debug/prAssert.h"
#include "../core/prDefines.h"


// Class: prSingleton
//      Contains a simple singleton class for singletons that don't need to exist at startup.
template<typename T>
class prSingleton
{
public:
    // Method: GetInstance
    //      Returns a reference to this instance.
    static T *GetInstance()
    {
        if (Instance() == NULL)
        {
            SingletonCreate();
        }

        return Instance();
    }

    // Method: SingletonCreate
    //      Create the singleton.
    static void *SingletonCreate() 
    { 
        if (Instance() == NULL)
        {
            Instance() = new T(); 
        }
        
        return Instance();
    }

    // Method: SingletonDestroy
    //      Destroy the singleton.
    static void SingletonDestroy()
    {
        PRSAFE_DELETE(Instance()); 
    }

    // Method: SingletonExists
    //      Determines if the singleton exists.
    static bool SingletonExists()
    {
        return (Instance() != NULL);
    }


protected:

    // Method: Instance
    //      Returns a reference to this instance.
    static T *& Instance()
    {
        static T *instance = NULL;
        return instance;
    }
};


// This macro protects a singleton from duplication.
#define PRMAKE_SINGLETON(cls)                                           \
private:                                                                \
    friend class prSingleton<cls>;                                      \
                                                                        \
    /* Private ctor/dtor stops client from creating an instance. */     \
    cls();                                                              \
    ~cls();                                                             \
                                                                        \
    /* Stops passing by value and assignment. */                        \
    cls(const cls&);                                                    \
    const cls& operator = (const cls&);


#endif// __PRSINGLETON_H
