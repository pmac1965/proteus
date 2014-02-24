// ----------------------------------------------------------------------------
//
// File: prWidget.cpp
//
//      Description     - Contains base GUI widget class
//      Author          - Paul Michael McNab.
//      Copyright       - Copyright Paul Michael McNab. All rights reserved.
//
// Disclaimer:
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//      PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//      EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//      PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//      PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//      LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//      NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ----------------------------------------------------------------------------
/*

#include "prWidget.h"
#include "../core/stringUtil.h"
#include "../core/macros.h"
#include "../debug/prDebug.h"
//#include "../display/spriteManager.h"


// ----------------------------------------------------------------------------
// Ctor
// ----------------------------------------------------------------------------
prWidget::prWidget(WidgetType type, const char *name, SpriteManager *pSpriteManager) 
    : m_type            (type)
    , m_pSpriteManager  (pSpriteManager)
    , m_colour          (Colour::White)
{
    ASSERT(pSpriteManager);

    // Set widget name
    const char *theName = "Unnamed widget";

    // New name?
    if (name && *name)
    {
        theName = name;
    }

    m_name.Set(theName);

    // Init data
    m_visible   = false;
    m_enabled   = false;
    m_animated  = false;
    m_active    = true;
    m_destroy   = false;
    m_exp0      = false;
    m_exp1      = false;
    m_exp2      = false;
}


// ----------------------------------------------------------------------------
// Dtor
// ----------------------------------------------------------------------------
prWidget::~prWidget()
{
}
*/
