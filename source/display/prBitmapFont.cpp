/**
 * prBitmapFont.cpp
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


// Platform
#if defined(PLATFORM_PC)
  #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
  #endif
  
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  
  #include <string.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <string.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#else
  #error No platform defined.

#endif


#include <stdarg.h>
#include "prBitmapFont.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../file/prFile.h"
#include "../file/prFileShared.h"
#include "../tinyxml/tinyxml.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../core/prCore.h"
#include "../core/prResourceManager.h"
#include "../utf8proc/utf8proc.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prOglUtils.h"


//using namespace Proteus::Core;


// Defines
#define MSG_BUFFER_SIZE     1024
//#define DEBUG_BMAP_FONT


// Info about a character.
typedef struct CharData
{
    s32 id;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 xoffset;
    s32 yoffset;
    s32 xadvance;
    s32 page;
    s32 chnl;

} CharData;


// Info about a page
typedef struct PageData
{
    s32        id;
    char       filename[FILE_MAX_FILENAME_SIZE];
    prTexture *pTexture;

} PageData;


// Kerning data
typedef struct KerningData
{
    u32 first;
    u32 second;
    s32 amount;

} KerningData;


// Implementation data.
typedef struct BitmapFontImplementation
{
    // ------------------------------------------------------------------------
    // Ctor
    // ------------------------------------------------------------------------
    BitmapFontImplementation()
    {
        pages        = 0;
        characters   = 0;
        boundPage    = -1;
        lineHeight   = 0;
        base         = 0;
        scaleW       = 0;
        scaleH       = 0;
        kernings     = 0;
        pCharInfo    = nullptr;
        pPageInfo    = nullptr;
        pKerningData = nullptr;
        rotation     = 0.0f;
    }

    
    // ------------------------------------------------------------------------
    // Dtor
    // ------------------------------------------------------------------------
    ~BitmapFontImplementation()
    {
        PRSAFE_DELETE_ARRAY(pCharInfo);
        PRSAFE_DELETE_ARRAY(pPageInfo);
        PRSAFE_DELETE_ARRAY(pKerningData);
    }


    // ------------------------------------------------------------------------
    // Load the font data.
    // ------------------------------------------------------------------------
    void Load(const char *filename)
    {
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
            }

            delete doc;
        }
    }


    // ------------------------------------------------------------------------
    // Parses the xml file.
    // ------------------------------------------------------------------------
    void ParseFile(TiXmlNode* pParent)
    {
        switch (pParent->Type())
        {
        case TiXmlNode::TINYXML_ELEMENT:
            {
                if (strcmp(pParent->Value(), "info") == 0)
                {
                    ParseInfo(pParent->ToElement());
                }
                else if (strcmp(pParent->Value(), "common") == 0)
                {
                    ParseCommon(pParent->ToElement());
                }
                else if (strcmp(pParent->Value(), "pages") == 0)
                {
                    ParsePages(pParent->ToElement());
                }
                else if (strcmp(pParent->Value(), "chars") == 0)
                {
                    ParseChars(pParent->ToElement());
                }
                else if (strcmp(pParent->Value(), "kernings") == 0)
                {
                    ParseKernings(pParent->ToElement());
                }
                // Unused
                else if (strcmp(pParent->Value(), "char")    == 0 ||
                         strcmp(pParent->Value(), "page")    == 0 ||
                         strcmp(pParent->Value(), "font")    == 0 ||
                         strcmp(pParent->Value(), "kerning") == 0)
                {
                    // Parsed by above.
                }
                else
                {
                    prTrace(prLogLevel::LogError, "Unsupported element: %s\n", pParent->Value());
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


    // ------------------------------------------------------------------------
    // Parses the info element
    // ------------------------------------------------------------------------
    void ParseInfo(TiXmlElement* pElement)
    {
        PRASSERT(pElement);
        PRUNUSED(pElement);

#if defined(DEBUG_BMAP_FONT)
        if (pElement)
        {
            const char * face     = pElement->Attribute("face");
            const char * size     = pElement->Attribute("size");
            const char * bold     = pElement->Attribute("bold");
            const char * italic   = pElement->Attribute("italic");
            const char * charset  = pElement->Attribute("charset");
            const char * unicode  = pElement->Attribute("unicode");
            const char * stretchH = pElement->Attribute("stretchH");
            const char * smooth   = pElement->Attribute("smooth");
            const char * aa       = pElement->Attribute("aa");
            const char * padding  = pElement->Attribute("padding");
            const char * spacing  = pElement->Attribute("spacing");
            const char * outline  = pElement->Attribute("outline");
            prTrace(LogError, "face    : %s\n", face    );
            prTrace(LogError, "size    : %s\n", size    );
            prTrace(LogError, "bold    : %s\n", bold    );
            prTrace(LogError, "italic  : %s\n", italic  );
            prTrace(LogError, "charset : %s\n", charset );
            prTrace(LogError, "unicode : %s\n", unicode );
            prTrace(LogError, "stretchH: %s\n", stretchH);
            prTrace(LogError, "smooth  : %s\n", smooth  );
            prTrace(LogError, "aa      : %s\n", aa      );
            prTrace(LogError, "padding : %s\n", padding );
            prTrace(LogError, "spacing : %s\n", spacing );
            prTrace(LogError, "outline : %s\n", outline );
        }
#endif
    }


    // ------------------------------------------------------------------------
    // Parses the common element
    // ------------------------------------------------------------------------
    void ParseCommon(TiXmlElement* pElement)
    {
        PRASSERT(pElement);

        if (pElement)
        {
            const char * _lineHeight    = pElement->Attribute("lineHeight");
            const char * _base          = pElement->Attribute("base");
            const char * _scaleW        = pElement->Attribute("scaleW");
            const char * _scaleH        = pElement->Attribute("scaleH");
            const char * _pages         = pElement->Attribute("pages");

#if defined(DEBUG_BMAP_FONT)
            const char * packed     = pElement->Attribute("packed");
            const char * alphaChnl  = pElement->Attribute("alphaChnl");
            const char * redChnl    = pElement->Attribute("redChnl");
            const char * greenChnl  = pElement->Attribute("greenChnl");
            const char * blueChnl   = pElement->Attribute("blueChnl");
            prTrace(LogError, "lineHeight : %s\n", _lineHeight);
            prTrace(LogError, "base       : %s\n", _base      );
            prTrace(LogError, "scaleW     : %s\n", _scaleW    );
            prTrace(LogError, "scaleH     : %s\n", _scaleH    );
            prTrace(LogError, "pages      : %s\n", _pages     );
            prTrace(LogError, "packed     : %s\n", packed     ); // Unused
            prTrace(LogError, "alphaChnl  : %s\n", alphaChnl  ); // Unused
            prTrace(LogError, "redChnl    : %s\n", redChnl    ); // Unused
            prTrace(LogError, "greenChnl  : %s\n", greenChnl  ); // Unused
            prTrace(LogError, "blueChnl   : %s\n", blueChnl   ); // Unused
#endif

            PRASSERT(_lineHeight);
            PRASSERT(_base);
            PRASSERT(_scaleW);
            PRASSERT(_scaleH);
            PRASSERT(_pages);

            if (_lineHeight &&
                _base       &&
                _scaleW     &&
                _scaleH     &&
                _pages)
            {
                lineHeight  = atoi(_lineHeight);
                pages       = atoi(_pages);
                base        = atoi(_base);
                scaleW      = atoi(_scaleW);
                scaleH      = atoi(_scaleH);

                PRASSERT(pages > 0);                
                if (pages > 0)
                {
                    pPageInfo = new PageData[pages];
                }
            }
        }
    }


    // ------------------------------------------------------------------------
    // Parses any pages elements
    // ------------------------------------------------------------------------
    void ParsePages(TiXmlElement* pElement)
    {
        PRASSERT(pElement);

        if (pElement)
        {
            // Acquire the child frame data.
            TiXmlHandle root(pElement);
            TiXmlElement *pElem  = root.FirstChild("page").Element();

            s32 index = 0;
            for (; pElem; pElem = pElem->NextSiblingElement())
            {
                const char * id   = pElem->Attribute("id");
                const char * file = pElem->Attribute("file");

                PRASSERT(pPageInfo);
                if (pPageInfo)
                {
                    // Store page id.
                    PRASSERT(id);
                    if (id)
                    {
                        pPageInfo[index].id = atoi(id);
                    }


                    // Create the texture filename. Must be in 'data/fonts'
                    PRASSERT(file);
                    if (file)
                    {
                        strcpy(pPageInfo[index].filename, "data/fonts/");
                        strcat(pPageInfo[index].filename, file);
                        s32 idx = prStringFindLastIndex(pPageInfo[index].filename, '.');
                        strcpy(&pPageInfo[index].filename[idx + 1], "pvr");

                        // Create the page texture
                        prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
                        PRASSERT(pRM)
                        pPageInfo[index].pTexture = pRM->Load<prTexture>(pPageInfo[index].filename, false, TEXTRA_ANTIALIAS);
                        PRASSERT(pPageInfo[index].pTexture);
                    }
                }

                // Increment page count.
                index++;
            }
        }
    }


    // ------------------------------------------------------------------------
    // Parses any chars elements
    // ------------------------------------------------------------------------
    void ParseChars(TiXmlElement* pElement)
    {
        PRASSERT(pElement);
        if (pElement)
        {
            const char *count = pElement->Attribute("count");            
            PRASSERT(count);
            if (count)
            {
                // Get the number of characters in this font.
                characters = atoi(count);
                if (characters == 0)
                {
                    prTrace(prLogLevel::LogError, "Font character count is invalid\n");
                    return;
                }

                
                // Get char data.
                pCharInfo = new CharData[characters];
                PRASSERT(pCharInfo);
                if (pCharInfo)
                {
                    // Acquire character data.
                    TiXmlHandle   root(pElement);
                    TiXmlElement *pElem = root.FirstChild("char").Element();

                    s32 index = 0;
                    for (; pElem; pElem = pElem->NextSiblingElement())
                    {
                        const char *id       = pElem->Attribute("id");
                        const char *x        = pElem->Attribute("x");
                        const char *y        = pElem->Attribute("y");
                        const char *width    = pElem->Attribute("width");
                        const char *height   = pElem->Attribute("height");
                        const char *xoffset  = pElem->Attribute("xoffset");
                        const char *yoffset  = pElem->Attribute("yoffset");
                        const char *xadvance = pElem->Attribute("xadvance");
                        const char *page     = pElem->Attribute("page");
                        const char *chnl     = pElem->Attribute("chnl");
                    
                        // Store character info.
                        PRASSERT(index < characters);
                        PRASSERT(id);
                        PRASSERT(x);
                        PRASSERT(y);
                        PRASSERT(width);
                        PRASSERT(height);
                        PRASSERT(xoffset);
                        PRASSERT(yoffset);
                        PRASSERT(xadvance);
                        PRASSERT(page);
                        PRASSERT(chnl);

                        if (index < characters)
                        {
                            pCharInfo[index].id       = atoi(id);
                            pCharInfo[index].x        = atoi(x);
                            pCharInfo[index].y        = atoi(y);
                            pCharInfo[index].width    = atoi(width);
                            pCharInfo[index].height   = atoi(height);
                            pCharInfo[index].xoffset  = atoi(xoffset);
                            pCharInfo[index].yoffset  = atoi(yoffset);
                            pCharInfo[index].xadvance = atoi(xadvance);
                            pCharInfo[index].page     = atoi(page);
                            pCharInfo[index].chnl     = atoi(chnl);                        
                            index++;
                        }
                    }
                }
            }
        }
    }


    // ------------------------------------------------------------------------
    // Parses any kernings elements.
    // ------------------------------------------------------------------------
    void ParseKernings(TiXmlElement* pElement)
    {
        PRASSERT(pElement);
        if (pElement)
        {
            const char *count = pElement->Attribute("count");            
            PRASSERT(count);
            if (count)
            {
                // Get the number of kerning pairs in this font.
                kernings = atoi(count);
                if (kernings == 0)
                {
                    prTrace(prLogLevel::LogError, "Font kernings count is zero\n");
                }

                // Extract kerning data
                pKerningData = new KerningData[kernings];
                PRASSERT(pKerningData);
                if (pKerningData)
                {
                    // Acquire character data.
                    TiXmlHandle   root(pElement);
                    TiXmlElement *pElem = root.FirstChild("kerning").Element();
                    PRASSERT(pElem);

                    s32 index = 0;
                    for (; pElem; pElem = pElem->NextSiblingElement())
                    {
                        const char *first  = pElem->Attribute("first");
                        const char *second = pElem->Attribute("second");
                        const char *amount = pElem->Attribute("amount");
                    
                        // Store kerning info.
                        PRASSERT(first);
                        PRASSERT(second);
                        PRASSERT(amount);
                        PRASSERT(index < kernings);

                        if (index < kernings)
                        {
                            pKerningData[index].first  = atoi(first);
                            pKerningData[index].second = atoi(second);
                            pKerningData[index].amount = atoi(amount);
                            index++;
                        }
                    }
                }
            }
        }
    }


    // ------------------------------------------------------------------------
    // Gets kerning data
    // ------------------------------------------------------------------------
    f32 GetKerning(u32 first, u32 second) const
    {
        f32 amount = 0.0f;

        if (pKerningData != nullptr)
        {
		    s32 kern = 0;
		    while(kern < kernings && pKerningData[kern].first <= first)
		    {
			    if (pKerningData[kern].first == first && pKerningData[kern].second == second)
                {
				    return (float)pKerningData[kern].amount;
                }

			    kern++;
		    }
        }

        return amount;
    }


    // Data
    s32              pages;
    s32              characters;
    s32              kernings;
    s32              boundPage;
    s32              lineHeight;
    s32              base;
    s32              scaleW;
    s32              scaleH;
    f32              rotation;

    CharData        *pCharInfo;
    PageData        *pPageInfo;
    KerningData     *pKerningData;

} BitmapFontImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prBitmapFont::prBitmapFont() : pImpl (new BitmapFontImplementation())
                             , imp   (*pImpl)
{
    PRASSERT(pImpl);
    TODO("Add texture unload")
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prBitmapFont::~prBitmapFont()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Loads the font data.
/// ---------------------------------------------------------------------------
void prBitmapFont::Load(const char *filename)
{
    PRASSERT(filename && *filename);
    PRASSERT(pImpl);
    imp.Load(filename);
}


/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void prBitmapFont::Draw(f32 x, f32 y, const char *fmt, ...)
{
    char message[MSG_BUFFER_SIZE];

	// Format the output.
    va_list args;
    va_start(args, fmt);        

    vsprintf(message, fmt, args);

    va_end(args);

    Draw(x, y, 1.0f, prColour::White, ALIGN_LEFT, message);
}


/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void prBitmapFont::Draw(f32 x, f32 y, float scale, prColour colour, s32 alignment, const char *fmt, ...)
{
    if (fmt && *fmt)
    {
        char message[MSG_BUFFER_SIZE];


		// Format the output.
        va_list args;
        va_start(args, fmt);        
        vsprintf(message, fmt, args);
        va_end(args);


        // Render init
        ERR_CHECK(glEnable(GL_BLEND));
        //ERR_CHECK();
        ERR_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        //ERR_CHECK();

        // Set states
        //glEnableClientState(GL_VERTEX_ARRAY);
        //ERR_CHECK();
        ERR_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
        //ERR_CHECK();


        f32 xbak = x;

        // Set alignment
        switch(alignment)
        {
        default:
            prTrace(prLogLevel::LogError, "Unsupported alignment\n");
            break;

        case ALIGN_LEFT:
            break;

        case ALIGN_RIGHT:
            x -= MeasureString(message, scale).x;
            break;

        case ALIGN_CENTER:
            x -= MeasureStringUntilTerm(message, scale).x / 2;
            break;
        }


        // Move to position
        glPushMatrix();
        glTranslatef(x, y, 0);


        // Set the colour.
        glColor4f(colour.red, colour.green, colour.blue, colour.alpha);


        // And rotate.
        glRotatef(imp.rotation, 0, 0, 1);


        // Write the text.
        imp.boundPage = -1;

        s32      c;
        s32      index = 0;
        s32      character;
        float    dist  = 0;
        float    w     = (1.0f / imp.scaleW);       // Pre-calculated value
        float    h     = (1.0f / imp.scaleH);       // Pre-calculated value
        float    ya    = 0;

        do
        {
            c = message[index];

            // utf-8?
            if (c < 0 || c > 127)
            {
                index += (s32)utf8proc_iterate((const ::uint8_t*)&fmt[index], -1, (::int32_t*)&character);
            }
            else
            {
                character = c;
                index++;
            }

            if (character == '\n')
            {
                dist = 0;
                ya  += (imp.lineHeight * scale);

                // Set alignment
                switch(alignment)
                {
                default:
                    prTrace(prLogLevel::LogError, "Unsupported alignment\n");
                    break;

                case ALIGN_LEFT:
                    break;

                case ALIGN_RIGHT:
                    x  = xbak;
                    x -= MeasureStringUntilTerm(&message[index], scale).x;
                    break;

                case ALIGN_CENTER:
                    x  = xbak;
                    x -= MeasureStringUntilTerm(&message[index], scale).x / 2;
                    break;
                }
            }
            else if (character == '\r') // Just in case, we only need the one CR/LF character
            {
            }
            else if (character != '\0')
            {
                // find character details
                s32  lower  = 0;
                s32  upper  = imp.characters - 1;
                s32  mid    = 0;
                bool found  = false;

                while(lower <= upper)
                {
                    mid = (lower + upper) / 2;

                    if (character > imp.pCharInfo[mid].id)
                    {
                        lower = mid + 1;
                    }
                    else if (character < imp.pCharInfo[mid].id)
                    {
                        upper = mid - 1;
                    }
                    else
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    CharData *pCharInfo = &imp.pCharInfo[mid];

                    if (character != ' ')
                    {
                        // Bind the current page texture.
                        s32 currentPage = pCharInfo->page;                    
                        PRASSERT(currentPage >= 0);
                        PRASSERT(currentPage < imp.pages);

                        if (imp.boundPage != currentPage)
                        {
                            imp.pPageInfo[currentPage].pTexture->Bind();
                            imp.boundPage = currentPage;
                        }

                        float width  = (float)(pCharInfo->width);
                        float height = (float)(pCharInfo->height);

                        float u0 = pCharInfo->x * w;
                        float u1 = u0 + (width * w);
                        float v0 = 1.0f - ((pCharInfo->y * h) + (height * h));
                        float v1 = v0 + (height * h);


                        ERR_CHECK(glPushMatrix());
                        //ERR_CHECK();

                        float px = pCharInfo->xoffset + (width  / 2) + imp.GetKerning(character, message[index]) + dist;
                        float py = pCharInfo->yoffset + (height / 2);

                        ERR_CHECK(glTranslatef(px * scale, (py * scale) + ya, 0));
                        //ERR_CHECK();

                        ERR_CHECK(glScalef(scale * width, scale * height, 1));
                        //ERR_CHECK();

                        QuadData quadData[] =
                        {
                            {-0.5f,  0.5f, u0, v0, },
                            {-0.5f, -0.5f, u0, v1, },
                            { 0.5f,  0.5f, u1, v0, },
                            { 0.5f, -0.5f, u1, v1, },
                        };

                        ERR_CHECK(glVertexPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->x));
                        //ERR_CHECK();

                        ERR_CHECK(glTexCoordPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->u));
                        //ERR_CHECK();

                        ERR_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
                        //ERR_CHECK();
                    
                        ERR_CHECK(glPopMatrix());
                        //ERR_CHECK();

                        PRUNUSED(quadData->v);
                    }

                    dist += pCharInfo->xadvance;
                }
                else
                {
                    prTrace(prLogLevel::LogError, "Unsupported character: %i, %c\n", character, character);
                }
            }
        }
        while(c != '\0');


        // Reset states
        ERR_CHECK(glPopMatrix());
        //ERR_CHECK();

        ERR_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
        //ERR_CHECK();

        //glDisableClientState(GL_VERTEX_ARRAY);
        //ERR_CHECK();

        ERR_CHECK(glDisable(GL_BLEND));
        //ERR_CHECK();
    }
}


/// ---------------------------------------------------------------------------
/// Returns the pixel length of the string.
/// ---------------------------------------------------------------------------
Proteus::Math::prVector2 prBitmapFont::MeasureString(const char *string, float scale)
{
    f32 max = 0.0f;
    Proteus::Math::prVector2 len = Proteus::Math::prVector2(0, (f32)imp.lineHeight);

    if (string && *string)
    {
        s32   c;
        s32   index = 0;
        s32   character;
        
        // --
        do
        {
            c = string[index];

            // utf-8?
            if (c < 0 || c > 127)
            {
                 index += (s32)utf8proc_iterate((const ::uint8_t*)&string[index], -1, (::int32_t*)&character);
            }
            else
            {
                character = c;
                index++;
            }

            if (character == '\n')
            {
                len.x  = 0.0f;
                len.y += imp.lineHeight;
            }
            else if (character == '\r') // Just in case, we only need the one CR/LF character
            {
            }
            else if (character != '\0')
            {
                // find character details
                s32  lower  = 0;
                s32  upper  = imp.characters - 1;
                s32  mid    = 0;
                bool found  = false;

                while(lower <= upper)
                {
                    mid = (lower + upper) / 2;

                    if (character > imp.pCharInfo[mid].id)
                    {
                        lower = mid + 1;
                    }
                    else if (character < imp.pCharInfo[mid].id)
                    {
                        upper = mid - 1;
                    }
                    else
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    len.x += imp.pCharInfo[mid].xadvance;

                    if (len.x > max)
                    {
                        max = len.x;
                    }
                }
                else
                {
                    prTrace(prLogLevel::LogError, "Unsupported character: %i, %c\n", character, character);
                }
            }
        }
        while(c != '\0');
    }

    len.x = max;
    len  *= scale;
    
    return len;
}


/// ---------------------------------------------------------------------------
/// Allows the text to be rotated, for sideways text, etc.
/// ---------------------------------------------------------------------------
void prBitmapFont::SetRotation(f32 rotation)
{
    imp.rotation = rotation;
}


/// ---------------------------------------------------------------------------
/// Returns the pixel length of the string.
/// ---------------------------------------------------------------------------
Proteus::Math::prVector2 prBitmapFont::MeasureStringUntilTerm(const char *string, float scale)
{
    f32     max = 0.0f;
    Proteus::Math::prVector2 len = Proteus::Math::prVector2(0, (f32)imp.lineHeight);

    if (string && *string)
    {
        char  c;
        s32   index = 0;
        s32   character;
        
        do
        {
            c = string[index];

            // utf-8?
            if (c < 0)// || c > 127)
            {
                index += (s32)utf8proc_iterate((const ::uint8_t*)&string[index], -1, (::int32_t*)&character);
            }
            else
            {
                character = c;
                index++;
            }


            // Get size.
            if (character != '\0' && character != '\n' && character != '\r')
            {
                // find character details
                s32  lower  = 0;
                s32  upper  = imp.characters - 1;
                s32  mid    = 0;
                bool found  = false;

                while(lower <= upper)
                {
                    mid = (lower + upper) / 2;

                    if (character > imp.pCharInfo[mid].id)
                    {
                        lower = mid + 1;
                    }
                    else if (character < imp.pCharInfo[mid].id)
                    {
                        upper = mid - 1;
                    }
                    else
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    len.x += imp.pCharInfo[mid].xadvance;

                    if (len.x > max)
                    {
                        max = len.x;
                    }
                }
                else
                {
                    prTrace(prLogLevel::LogError, "Unsupported character: %i, %c\n", character, character);
                }
            }
        }
        while(c != '\0' && c != '\n' && c != '\r');
    }

    len.x = max;
    len  *= scale;
    
    return len;
}
