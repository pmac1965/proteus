// File: prVertex.h
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


#ifndef __PRVERTEX_H
#define __PRVERTEX_H


#include "prTypes.h"


// Typedef: prVertex2D
//      2 dimensional vertex type
typedef struct prVertex2D
{
    f32 x;
    f32 y;

} prVertex2D;


// Typedef: prVertex3D
//      3 dimensional vertex type
typedef struct prVertex3D
{
    f32 x;
    f32 y;
    f32 z;

} prVertex3D;


#endif//__PRVERTEX_H
