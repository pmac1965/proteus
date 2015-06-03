/**
 * prTagParser.cpp
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


#include "prTagParser.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include <cstring>


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


//#define TAG_DEBUG
#define TAG_MAX_SIZE    128


using namespace Proteus::Core;


// Local data
namespace
{
    // Parsers
    bool ParamParserSetRGB(const char *string);

    // Last tag
    char tag[TAG_MAX_SIZE] = { '\0' };

    // Parser for parameters
    typedef bool (*TagParamParser)(const char *);

    // The tags
    struct TextTags
    {
        const char     *tag;
        u32             action;
        u32             length;     
        TagParamParser  parser;
    }
    tags [] = 
    {
        { "set_rgb", PRTT_SET_RGB, 7, ParamParserSetRGB },
    };

    
    // Data
    f32 r           = 0.0f;         // Internal data for set RGB
    f32 g           = 0.0f;         // Internal data for set RGB
    f32 b           = 0.0f;         // Internal data for set RGB
    f32 a           = 0.0f;         // Internal data for set RGB
    s32 tagLength   = 0;            // Length of tag found
    u32 tagFound    = PRTT_NONE;    // Last tag found


    // Parser for parameters
    bool ParamParserSetRGB(const char *pString)
    {
        bool result = false;

        if (pString && *pString)
        {
            s32 commas = prStringCountCharsOfType(pString, ',');

            switch(commas)
            {
            // 2 commas == 3 parameters (0,0,0)
            case 2:
                prStringParseFloats(pString + 1, r, g, b);
                break;

            // 3 commas == 4 parameters (0,0,0,0)
            case 3:
                prStringParseFloats(pString + 1, r, g, b, a);
                break;

            default:
                PRPANIC("Invalid parameter list: %s", pString);
            }

            #if defined(TAG_DEBUG)
            prTrace(LogError, "Parsed: commas %i, R:%g, G:%g, B:%g, A:%g\n", commas, r, g, b, a);
            #endif


            result = true;
        }
        
        return result;
    }

    // Internal function to set the RGBA colour
    void prTagSetRGB()
    {
        glColor4f(r, g, b, a);
    }
};


/// ---------------------------------------------------------------------------
/// Checks if a # in a string message indicates a tag
/// ---------------------------------------------------------------------------
u32 prTagIsTag(const char *pString, f32 _a)
{
    // Reset
    tagLength   = 0;
    tagFound    = PRTT_NONE;
    r           = 1.0f;
    g           = 1.0f;
    b           = 1.0f;
    a           = _a;                   // Allows alpha not to be destroyed

    if (pString && *pString)
    {
        // Start at 1 and bypass starting #
        for (s32 count=1; count<TAG_MAX_SIZE; count++)
        {
            char c = pString[count];

            // String terminating?
            if (c != '\0')
            {
                if (c != '#')
                {
                    // Store
                    tag[count - 1] = c;
                }
                else
                {
                    // Terminate
                    tag[count - 1] = '\0';

                    #if defined(TAG_DEBUG)
                    prTrace(LogError, "SEARCH TAG?: '%s'\n", tag);
                    #endif

                    for (u32 i=0; i<PRARRAY_SIZE(tags); i++)
                    {
                        if (strncmp(tags[i].tag, tag, tags[i].length) == 0)
                        {
                            #if defined(TAG_DEBUG)
                            prTrace(LogError, "Found: >>%s<<\n", tags[i].tag);
                            #endif
                            
                            if (tags[i].parser)
                            {
                                // Pass in the parameter string
                                if (tags[i].parser(&tag[tags[i].length]))
                                {
                                    tagLength   = (s32)strlen(tag) + 1;  // Add for end hash
                                    tagFound    = tags[i].action;
                                    break;
                                }
                                else
                                {
                                    PRPANIC("Failed to parse tag correctly");
                                }
                            }
                            else
                            {
                                PRPANIC("No parser for tag");
                            }
                        }
                    }

                    break;
                }
            }
        }
    }

    return tagFound;
}


/// ---------------------------------------------------------------------------
/// Performs the last action found 
/// ---------------------------------------------------------------------------
void prTagDoAction()
{
    switch(tagFound)
    {
    case PRTT_SET_RGB:
        prTagSetRGB();
        break;
    }

    // Clear tag
    tagFound = PRTT_NONE;
}


/// ---------------------------------------------------------------------------
/// Returns the length of the last tag found
/// ---------------------------------------------------------------------------
s32 prTagGetTagLength()
{
    return tagLength;
}
