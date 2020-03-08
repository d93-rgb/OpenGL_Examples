#include "graphicscontext.h"
#include "GL/gl3w.h"

namespace ogl_examples
{

GraphicsContext::GraphicsContext(int screen_width, int screen_height)
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, screen_width, screen_height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

} // namespace ogl_examples