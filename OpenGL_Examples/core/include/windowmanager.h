#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include "opengl_examples.h"
#include "renderingmanager.h"
#include "graphicscontext.h"

namespace ogl_examples
{

class WindowManager
{
public:
	GLuint screen_width;
	GLuint screen_height;

	WindowManager(GLuint screen_width, GLuint screen_height);

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

	bool mouse_button_pressed = false;
	double old_x_pos;
	double old_y_pos;
	double new_x_pos;
	double new_y_pos;
	double x_pos_diff;
	double y_pos_diff;

};


} // namespace ogl_examples