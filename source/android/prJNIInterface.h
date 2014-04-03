// File: prJNIInterface.h
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


#ifndef __PRJNIINTERFACE_H
#define __PRJNIINTERFACE_H


//#define FX_PLAYING  0
//#define FX_PAUSED   1


// Function: prJNISetPackageName
//      Sets the android package name
void prJNISetPackageName(const char *package);

// Function: prJNISetActivityName
//      Sets the android activity name
void prJNISetActivityName(const char *activity);

// Function: prJNISetProviderName
//      Sets the android provider name
void prJNISetProviderName(const char *provider);

// Function: prJNISetAnalyticName
//      Sets the android analytic name
void prJNISetAnalyticName(const char *analytic);


#endif//_JNIINTERFACE_H

