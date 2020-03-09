#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include "opengl_examples.h"

namespace ogl_examples
{

class WindowManager
{
public:
	GLuint screen_width;
	GLuint screen_height;

	WindowManager(GLuint screen_width, GLuint screen_height);
	~WindowManager();

	void run();

	void set_renderering_manager(const std::shared_ptr<RenderingManager>& new_rm);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);

protected:
	GLFWwindow* window;

	std::shared_ptr<RenderingManager> rm;
	std::unique_ptr<GraphicsContext> gc;

};


} // namespace ogl_examples