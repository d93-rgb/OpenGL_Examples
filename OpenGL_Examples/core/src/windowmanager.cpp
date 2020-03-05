#include "windowmanager.h"

namespace ogl_examples
{
int current_scene_flag = 0;
int old_scene_flag = 0;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		current_scene_flag = (current_scene_flag + 1) % 2;
	}
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

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (gl3wInit()) {
		LOG(ERROR) << "failed to initialize OpenGL";
		std::exit(1);
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, screen_width, screen_height);

	rm.reset(new RenderingManager());
}

void WindowManager::run()
{
	glfwShowWindow(window);
	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();

		rm->run();
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void WindowManager::set_renderering_manager(const std::shared_ptr<RenderingManager>& new_rm)
{
	this->rm = new_rm;
}

} // namespace ogl_examples