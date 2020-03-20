#include "graphicscontext.h"

namespace ogl_examples
{

GraphicsContext::GraphicsContext(int screen_width, int screen_height)
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, screen_width, screen_height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glLineWidth(0.4f);
}

GraphicsContext::~GraphicsContext() = default;

void GraphicsContext::message_callback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
    std::exit(1);
}

} // namespace ogl_examples