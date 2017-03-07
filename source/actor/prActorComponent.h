// File: prActorComponent.h
//      The base interface class for actor components.
/**
 *  Copyright 2016 Paul Michael McNab
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


typedef struct ActorComponentData
{
    Proteus::Core::s32      mType;
    void                   *pData;

} ActorComponentData;


// Namespaces
namespace Proteus {
namespace Actor {


class prActorComponent
{
public:
    prActorComponent(){}
    virtual ~prActorComponent(){}

    void Initialize(ActorComponentData *pComponentData)
    {}
};


}}// Namespaces
