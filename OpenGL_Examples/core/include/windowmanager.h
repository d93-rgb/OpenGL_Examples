#pragma once
#include "opengl_examples.h"
#include "GL/gl3w.h"

namespace ogl_examples
{

class WindowManager
{
public:
	WindowManager(GLuint screen_width, GLuint screen_height);

private:
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	const GLint screenWidth = 800;
	const GLint screenHeight = 600;

	GLuint screen_width;
	GLuint screen_height;
};


} // namespace ogl_examples