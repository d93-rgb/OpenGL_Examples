#include "renderingmanager.h"
#include "windowmanager.h"
#include "graphicscontext.h"
#include "eventhandler.h"

namespace ogl_examples
{

int current_scene_flag = 0;
int old_scene_flag = 0;

void WindowManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

WindowManager::~WindowManager() = default;

void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void WindowManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	WindowManager* w = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	w->rm->get_current_renderer()->eh->handle_mouse_button(window, button, action, mods);
}

void WindowManager::mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{

	WindowManager* w = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	w->rm->get_current_renderer()->eh->handle_mouse(window, x_pos, y_pos);
}


WindowManager::WindowManager(GLuint screen_width, GLuint screen_height) :
	screen_width(screen_width), screen_height(screen_height)
{
	int error_code;
	const char* error_description;

	if (glfwInit() == GLFW_FALSE)
	{
		error_code = glfwGetError(&error_description);

		LOG(ERROR) << error_description;
		std::exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(
		screen_width,
		screen_height,
		"Rendering-Triangle Example",
		nullptr,
		nullptr);

	if (window == nullptr) {
		error_code = glfwGetError(&error_description);

		LOG(ERROR) << error_description;
		glfwTerminate();
		std::exit(1);
	}
	center_glfw_window(window, glfwGetPrimaryMonitor());

	glfwSetWindowUserPointer(window, this);

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (gl3wInit()) {
		LOG(ERROR) << "failed to initialize OpenGL";
		std::exit(1);
	}

	gc.reset(new GraphicsContext(screen_width, screen_height));
}

void WindowManager::run()
{
	glfwShowWindow(window);
	while (!glfwWindowShouldClose(window)) {
		//glfwPollEvents(); // high CPU usage
		//glfwWaitEventsTimeout(0.01); // medium CPU usage
		glfwWaitEvents(); // almost no CPU usage

		rm->run();

		glfwSwapBuffers(window);
	}

	rm->clean();
	glfwTerminate();
}

void WindowManager::set_renderering_manager(const std::shared_ptr<RenderingManager>& new_rm)
{
	this->rm = new_rm;
}

void WindowManager::center_glfw_window(GLFWwindow* window, GLFWmonitor* monitor)
{
	int error_code;
	const char* error_description;
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (!mode)
	{
		error_code = glfwGetError(&error_description);

		LOG(ERROR) << error_description;
		glfwTerminate();
		std::exit(1);
	}

	// center window
	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window,
		monitorX + (mode->width - windowWidth) / 2,
		monitorY + (mode->height - windowHeight) / 2);

}

} // namespace ogl_examples