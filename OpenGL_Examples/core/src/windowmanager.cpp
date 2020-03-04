#include "windowmanager.h"

namespace ogl_examples
{

WindowManager::WindowManager(GLuint screen_width, GLuint screen_height)
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
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(
		screenWidth,
		screenHeight,
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (gl3wInit()) {
		LOG(ERROR) << "failed to initialize OpenGL";
		std::exit(1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
}

void WindowManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

} // namespace ogl_examples