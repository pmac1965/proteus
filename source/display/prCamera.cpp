/**
 * prCamera.cpp
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
 *
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)
    // Exclude MFC
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
    #endif

    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
    #include "../display/prLookAt.h"

#elif defined(PLATFORM_IOS)
  #include "../display/prLookAt.h"

#elif defined(PLATFORM_MAC)
  #include "../display/prLookAt.h"

#elif defined(PLATFORM_BADA)
  #include "../display/prLookAt.h"

#elif defined(PLATFORM_ANDROID)
  #include "../display/prLookAt.h"

#elif defined(PLATFORM_LINUX)
  #include "../display/prLookAt.h"

#else
    #error Platform not defined.

#endif


#include "prCamera.h"
#include "../debug/prTrace.h"


// Statics
u32 prCamera::baseID = 1;


/// ---------------------------------------------------------------------------
/// Constructor
///
/// http://nehe.gamedev.net/article/using_gluunproject/16013/
/// http://www.gamedev.net/topic/583628-solved-gluproject/page__p__583628__hl__screen+to+world__fromsearch__1#entry583628
/// ---------------------------------------------------------------------------
prCamera::prCamera() 

: m_eye     (prVector3::Zero)
, m_at      (prVector3::Zero)
, m_up      (prVector3(0, -1, 0))

{
    m_active = PRFALSE;
    m_hash   = 0;
    m_id     = baseID++;
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prCamera::prCamera(prVector3 &eye, prVector3 &up, prVector3 &at) 

: m_eye     (eye)
, m_at      (at)
, m_up      (up)

{
    m_active = PRFALSE;
    m_hash   = 0;
    m_id     = baseID++;
}


/// ---------------------------------------------------------------------------
/// Destructor
/// ---------------------------------------------------------------------------
prCamera::~prCamera()
{
}


/// ---------------------------------------------------------------------------
/// Updates the camera.
/// ---------------------------------------------------------------------------
void prCamera::LookAt()
{
#if defined(PLATFORM_PC)

    gluLookAt
    (
        m_eye.x, m_eye.y, m_eye.z,
        m_at.x,  m_at.y,  m_at.z,
        m_up.x,  m_up.y,  m_up.z
    );

#else

    prLookAt
    (
        m_eye.x, m_eye.y, m_eye.z,
        m_at.x,  m_at.y,  m_at.z,
        m_up.x,  m_up.y,  m_up.z
    );

#endif
}


/// ---------------------------------------------------------------------------
/// Set target position.
/// ---------------------------------------------------------------------------
void prCamera::SetTarget(prVector3 &target)
{
    m_at.x = target.x;
    m_at.y = target.y;
    m_at.z = target.z;
}


/// ---------------------------------------------------------------------------
/// Set target position.
/// ---------------------------------------------------------------------------
void prCamera::SetTarget(const prVector3 &target)
{
    m_at.x = target.x;
    m_at.y = target.y;
    m_at.z = target.z;
}


/// ---------------------------------------------------------------------------
/// Get target position.
/// ---------------------------------------------------------------------------
prVector3 prCamera::GetTarget()
{
    return prVector3(m_at);
}


/// ---------------------------------------------------------------------------
/// Set camera position.
/// ---------------------------------------------------------------------------
void prCamera::SetPosition(prVector3 &pos)
{
    m_eye.x = pos.x;
    m_eye.y = pos.y;
    m_eye.z = pos.z;
}


/// ---------------------------------------------------------------------------
/// Set camera position.
/// ---------------------------------------------------------------------------
void prCamera::SetPosition(const prVector3 &pos)
{
    m_eye.x = pos.x;
    m_eye.y = pos.y;
    m_eye.z = pos.z;
}


/// ---------------------------------------------------------------------------
/// Get camera position.
/// ---------------------------------------------------------------------------
prVector3 prCamera::GetPosition()
{
    return prVector3(m_eye);
}
