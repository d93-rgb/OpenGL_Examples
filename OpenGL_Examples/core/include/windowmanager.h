#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include "opengl_examples.h"
#include "graphicscontext.h"

namespace ogl_examples
{

class WindowManager
{
public:
	WindowManager(GLuint screen_width, GLuint screen_height);

	void run();

	void set_renderering_manager(const std::shared_ptr<RenderingManager>& new_rm);

protected:
	GLuint screen_width;
	GLuint screen_height;

	GLFWwindow* window;

	std::shared_ptr<RenderingManager> rm;
	std::unique_ptr<GraphicsContext> gc;
};


} // namespace ogl_examples