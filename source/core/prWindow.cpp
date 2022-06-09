/**
 * prWindow.cpp
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


#include "../prConfig.h"
#include "prWindow.h"


// Ctor
prWindow::prWindow()
    : m_width     (0)
    , m_height    (0)
    , m_bits      (0)
    , m_minWidth  (0xFFFFFFFF)
    , m_minHeight (0xFFFFFFFF)
    , m_fullScreen(false)
    , m_active    (true)
    , m_exp0      (false)
    , m_exp1      (false)
{
}
