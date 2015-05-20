// File: prEncryption.h
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


#ifndef __PRENCRYPTION_H
#define __PRENCRYPTION_H


#include "../core/prTypes.h"


// Function: prEncrypt
//      Encrypts a block of data.
void prEncrypt(Proteus::Core::u8 *pData, Proteus::Core::u32 length);

// Function: prDecrypt
//      Decrypts a block of data.
void prDecrypt(Proteus::Core::u8 *pData, Proteus::Core::u32 length);


#endif//__PRENCRYPTION_H
