#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include "opengl_examples.h"
#include "renderingmanager.h"


namespace ogl_examples
{

class WindowManager
{
public:
	WindowManager(GLuint screen_width, GLuint screen_height);

	void run();

protected:
	GLuint screen_width;
	GLuint screen_height;

	GLFWwindow* window;

	std::shared_ptr<RenderingManager> rm;
};


} // namespace ogl_examples