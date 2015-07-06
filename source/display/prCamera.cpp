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

#elif defined(PLATFORM_ANDROID)
  #include "../display/prLookAt.h"

#elif defined(PLATFORM_LINUX)
  #include "../display/prLookAt.h"

#else
    #error Platform not defined.

#endif


#include "prCamera.h"
#include "../debug/prTrace.h"


using namespace Proteus::Math;
using namespace Proteus::Core;


// Statics
u32 prCamera::baseID = 1;


/// ---------------------------------------------------------------------------
/// Constructor
///
/// http://nehe.gamedev.net/article/using_gluunproject/16013/
/// http://www.gamedev.net/topic/583628-solved-gluproject/page__p__583628__hl__screen+to+world__fromsearch__1#entry583628
/// ---------------------------------------------------------------------------
prCamera::prCamera() 

: m_eye     (Proteus::Math::prVector3::Zero)
, m_up      (Proteus::Math::prVector3(0, 1, 0))
, m_at      (Proteus::Math::prVector3::Zero)

{
    m_active = PRFALSE;
    m_hash   = 0;
    m_id     = baseID++;
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prCamera::prCamera(const Proteus::Math::prVector3 &eye, const Proteus::Math::prVector3 &up, const Proteus::Math::prVector3 &at) 

: m_eye     (eye)
, m_up      (up)
, m_at      (at)

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
/// Rotates the camera.
/// ---------------------------------------------------------------------------
void prCamera::Rotate(f32 x, f32 y, f32 z)
{
	prVector3 vector = m_at - m_eye;

    if (x) 
	{
		m_at.z = (m_eye.z + sin(x)*vector.y + cos(x)*vector.z);
		m_at.y = (m_eye.y + cos(x)*vector.y - sin(x)*vector.z);
	}

	if (y) 
	{
	//prVector3 vector = m_at - m_eye;
		m_at.z = (m_eye.z + (sin(y) * vector.x) + (cos(y) * vector.z));
		m_at.x = (m_eye.x + (cos(y) * vector.x) - (sin(y) * vector.z));
	}

	if (z) 
	{
	//prVector3 vector = m_at - m_eye;
		m_at.x = (m_eye.x + sin(z)*vector.y + cos(z)*vector.x);		
		m_at.y = (m_eye.y + cos(z)*vector.y - sin(z)*vector.x);
	}
}


/// ---------------------------------------------------------------------------
/// Set target position.
/// ---------------------------------------------------------------------------
void prCamera::SetTarget(Proteus::Math::prVector3 &target)
{
    m_at.x = target.x;
    m_at.y = target.y;
    m_at.z = target.z;
}


/// ---------------------------------------------------------------------------
/// Set target position.
/// ---------------------------------------------------------------------------
void prCamera::SetTarget(const Proteus::Math::prVector3 &target)
{
    m_at.x = target.x;
    m_at.y = target.y;
    m_at.z = target.z;
}


/// ---------------------------------------------------------------------------
/// Set camera position.
/// ---------------------------------------------------------------------------
void prCamera::SetPosition(Proteus::Math::prVector3 &pos)
{
    m_eye.x = pos.x;
    m_eye.y = pos.y;
    m_eye.z = pos.z;
}


/// ---------------------------------------------------------------------------
/// Set camera position.
/// ---------------------------------------------------------------------------
void prCamera::SetPosition(const Proteus::Math::prVector3 &pos)
{
    m_eye.x = pos.x;
    m_eye.y = pos.y;
    m_eye.z = pos.z;
}
