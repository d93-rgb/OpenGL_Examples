#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include "opengl_examples.h"
#include "renderer.h"


namespace ogl_examples
{

class WindowManager
{
public:
	WindowManager(GLuint screen_width, GLuint screen_height);

	void run();

	void set_renderer(const std::shared_ptr<Renderer>& renderer);

protected:
	GLuint screen_width;
	GLuint screen_height;

	GLFWwindow* window;

	std::shared_ptr<Renderer> current_renderer;
};


} // namespace ogl_examples