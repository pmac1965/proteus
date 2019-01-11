// File: prEncryption.h
//      Performs a very simple form of XOR encryption. Designed to
//      stop people from messing with text data files easily.
//
// Notes:
//      If you want serious encryption, then you'll need to add it separately
//
// Notes:
//      Basic saving includes encyption.
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


#include "../core/prTypes.h"


// Function: prEncrypt
//      Encrypts a block of data.
//
// Parameters:
//      pData   - Address of the data block to encrypt
//      length  - Length of the data block to encrypt
void prEncrypt(u8 *pData, u32 length);

// Function: prDecrypt
//      Decrypts a block of data.
//
// Parameters:
//      pData   - Address of the data block to decrypt
//      length  - Length of the data block to decrypt
void prDecrypt(u8 *pData, u32 length);
