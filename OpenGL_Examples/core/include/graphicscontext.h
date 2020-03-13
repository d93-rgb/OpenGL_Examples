#pragma once
#include "opengl_examples.h"
#include "GL/gl3w.h"

namespace ogl_examples
{

class GraphicsContext
{
public:
	GraphicsContext(int screen_width, int screen_height);

	~GraphicsContext();

    static void message_callback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);

protected:

};

} // namespace ogl_examples