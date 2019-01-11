// File: prAccelerometer.h
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


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
struct AccelerometerImplementation;


// Class: prAccelerometer
//      A cross platform interface to acceleromter functionality
class prAccelerometer : public prCoreSystem
{
public:
    // Method: prAccelerometer
    //      Ctor
    prAccelerometer();

    // Method: ~prAccelerometer
    //      Dtor
    ~prAccelerometer();

    // Method: Update
    //      Ctor
    void Update(f32 x, f32 y, f32 z);
    
    // Get accelerometer values.
    void GetActual(f32 &x, f32 &y, f32 &z) const;

    // Get accelerometer values.
    void GetDifference(f32 &x, f32 &y, f32 &z) const;
   

private:    
    // Don't change order.
    AccelerometerImplementation  *pImpl;
    AccelerometerImplementation  &imp;


private:
    // Stops passing by value and assignment.
    prAccelerometer(const prAccelerometer&);
    const prAccelerometer& operator = (const prAccelerometer&);
};
