// File: prVertex.h
//      Basic vertex definitions.
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


#include "prTypes.h"
#include "../debug/prDebug.h" // For TODO


TODO("Move to math?? Its not math though!")


// Typedef: prVertex2D
//      2 dimensional vertex type
typedef struct prVertex2D
{
    f32 x;
    f32 y;

} prVertex2D;


// Typedef: prVertex3D
//      3 dimensional vertex type
typedef struct prVertex3D // https://gist.github.com/rishav007/accaf10c7aa06135d34ddf3919ebdb3b
{
    f32 x;
    f32 y;
    f32 z;

} prVertex3D;
