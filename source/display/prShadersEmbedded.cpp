#include "../prConfig.h"


#include <glew.h>


#if defined(PLATFORM_PC)
  #include <gl/GL.h>
#elif defined(PLATFORM_LINUX)
  #include <Gl/gl.h>
  #include <Gl/glu.h>
#endif

#include "prShadersEmbedded.h"
#include "prOglUtils.h"


//#include "shaders/fragment_shader.glsl"
//#include "shaders/vertex_shader.glsl"


const char* vertex_shader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;"
"layout(location = 1) in vec3 aColor;"
"layout(location = 2) in vec2 aTexCoord;"
"out vec3 ourColor;"
"out vec2 TexCoord;"
"void main()"
"{"
" gl_Position = vec4(aPos, 1.0);"
" ourColor = aColor;"
" TexCoord = aTexCoord;"
"}";


/*"# version 400\n"
"in vec3 vp;"
"void main() {"
" gl_Position = vec4(vp, 1.0);"
"}";//*/

const char* fragment_shader = 
"#version 330 core\n"
"out vec4 FragColor;"
"in vec3 ourColor;"
"in vec2 TexCoord;"
"uniform sampler2D ourTexture;"
""
"void main()"
"{"
"    FragColor = texture(ourTexture, TexCoord);"
"}";

/*"# version 400\n"
"out vec4 frag_colour;"
"void main() {"
" frag_colour = vec4(0.5, 0.5, 0.5, 1.0);"
"}";//*/


namespace
{
	GLuint shader_prg = 0;
}


void EmbeddedShadersInit()
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	ERR_CHECK();

	glShaderSource(vs, 1, &vertex_shader, nullptr);
	ERR_CHECK();
	glCompileShader(vs);
	ERR_CHECK();

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	ERR_CHECK();
	glShaderSource(fs, 1, &fragment_shader, nullptr);
	ERR_CHECK();
	glCompileShader(fs);
	ERR_CHECK();

	shader_prg = glCreateProgram();
	ERR_CHECK();
	glAttachShader(shader_prg, fs);
	ERR_CHECK();
	glAttachShader(shader_prg, vs);
	ERR_CHECK();
	glLinkProgram(shader_prg);
	ERR_CHECK();
}


void EmbeddedShadersUsePrg()
{
	glUseProgram(shader_prg);
	ERR_CHECK();
}

