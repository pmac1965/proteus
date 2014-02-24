/**
 * prBackground.cpp
 */


#include "../prConfig.h"


// ----------------------------------------------------------------------------
// Platform specifics
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#else
  #error No platform defined.

#endif


#include "prBackground.h"
#include "prRenderer.h"
#include "prTexture.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prResourceManager.h"
#include "../core/prStringUtil.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../tinyxml/tinyxml.h"
#include "../display/prOglUtils.h"


/// ---------------------------------------------------------------------------
/// Constructs the background.
/// ---------------------------------------------------------------------------
prBackground::prBackground(const char *filename) : m_colour(prColour::White)
{
    PRASSERT(filename && *filename);

    // Init data
    m_texture         = NULL;
    m_filename        = NULL;
    m_width           = -1;
    m_height          = -1;
    m_type            = prBackground::UNKNOWN;
    m_correctFileType = false;
    m_exp0            = false;
    m_exp1            = false;
    m_exp2            = false;
    m_pixelWidth      = 0.0f;
    m_pixelWidth      = 0.0f;
    m_scrnWidth       = 0.0f;
    m_scrnHeight      = 0.0f;
    m_v0              = 0.0f;
    m_u1              = 0.0f;
    m_yAdjust         = 0.0f;
    m_xAdjust         = 0.0f;

    TODO("Set bg type - m_type")

    // Parse the document
    TiXmlDocument* doc = new TiXmlDocument(filename);
    if (doc)
    {
        bool loaded = doc->LoadFile();      
        if (loaded)
        {
            ParseFile(doc);
        }
        else
        {
            PRWARN("Failed to Load %s\n", filename);
            return;
        }

        delete doc;
    }


    // Load the texture - filename acquired from the xml
    if (m_filename)
    {
        prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
        PRASSERT(pRM)
        m_texture = pRM->Load<prTexture>(m_filename);
        PRASSERT(m_texture);

        // Texture width
        m_width  = m_texture->GetWidth();
        m_height = m_texture->GetHeight();
        
        // Texture pixel size.
        m_pixelWidth  = 1.0f / m_width;
        m_pixelHeight = 1.0f / m_height;

        // Actual screen size.
        prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        PRASSERT(pReg)
        m_scrnWidth  = (float)atof(pReg->GetValue("ScreenWidth"));
        m_scrnHeight = (float)atof(pReg->GetValue("ScreenHeight"));

        // UV Coords pre-calc
        m_v0 = 1.0f - (m_pixelHeight * m_scrnHeight);
        m_u1 =        (m_pixelWidth  * m_scrnWidth);
        
        // Center adjust
        m_yAdjust = ((m_height - m_scrnHeight) / 2);
        m_xAdjust = ((m_width  - m_scrnWidth)  / 2);
    }
    else
    {
        PRPANIC("prBackground has no texture.");
    }
}


/// ---------------------------------------------------------------------------
/// Dtor.
/// ---------------------------------------------------------------------------
prBackground::~prBackground()
{
    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
    if (pRM && m_texture)
    {
        pRM->Unload(m_texture);
        m_texture = NULL;
    }

    PRSAFE_DELETE(m_filename);
}


/// ---------------------------------------------------------------------------
/// Draws the background.
/// ---------------------------------------------------------------------------
void prBackground::Draw()
{
    PRASSERT(m_texture);
    
    if (m_texture)
    {
        f32 width  = (f32)(m_width  >> 1);
        f32 height = (f32)(m_height >> 1);

        glPushMatrix();
        ERR_CHECK();

        // Translate to center
        glTranslatef(width, height, 0);
        ERR_CHECK();

        // Move to offset
        glTranslatef(pos.x - m_xAdjust, pos.y - m_yAdjust, 0);
        ERR_CHECK();

        // Set scale
        glScalef(m_scrnWidth, m_scrnHeight, 0);
        ERR_CHECK();

        if (m_texture->Bind())
        {
            prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
            if (pRenderer)
            {
                pRenderer->DrawQuad(0.0f, m_v0, m_u1, 1.0f, m_colour);
            }
        }

        glPopMatrix();
        ERR_CHECK();
    }
}


/// ---------------------------------------------------------------------------
/// Sets a tint colour.
/// ---------------------------------------------------------------------------
void prBackground::SetColour(prColour c)
{
    m_colour = c;
}


/// ---------------------------------------------------------------------------
/// Parses the xml file.
/// ---------------------------------------------------------------------------
void prBackground::ParseFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File data
            if (prStringCompare(pParent->Value(), "background_file") == 0)
            {
                ParseAttribs_File(pParent->ToElement());
            }
            // prBackground data
            else if (prStringCompare(pParent->Value(), "background") == 0)
            {
                ParseAttribs_Background(pParent->ToElement());
            }
        }
        break;

    default:
        break;
    } 


    for (TiXmlNode *pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        ParseFile(pChild);
    }
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information from the file like the version
///                    number.
/// ---------------------------------------------------------------------------
void prBackground::ParseAttribs_File(TiXmlElement* pElement)
{
    PRASSERT(pElement);

    if (pElement)
    {
        PRASSERT(pElement->Attribute("version"));
    }

    // Indicate correct file type.
    m_correctFileType = true;
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information about the background like its
///                    name, type, etc.
/// ---------------------------------------------------------------------------
void prBackground::ParseAttribs_Background(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    PRASSERT(m_correctFileType);

    if (pElement && m_correctFileType)
    {
        // Sanity checks
        PRASSERT(pElement->Attribute("name"));
        PRASSERT(pElement->Attribute("type"));


        // Acquire the child data.
        TiXmlHandle root(pElement);
        TiXmlElement *pElem = root.FirstChild("texture").Element();
        if (pElem)
        {
            // Get the bacgrounds texture data
            PRASSERT(pElem->Attribute("data"));
            
            int size = prStringLength(pElem->Attribute("data")) + 1;
            PRASSERT(size > 0);

            m_filename = new char[size];

            prStringCopySafe(m_filename, pElem->Attribute("data"), size);
        }
        else
        {
            PRPANIC("Background has no texture.");
        }
    }
}
