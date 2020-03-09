#include "renderingmanager.h"
#include "windowmanager.h"
#include "glm/gtc/matrix_transform.hpp"


namespace ogl_examples
{

int current_scene_flag = 0;
int old_scene_flag = 0;

bool OGL_EXAMPLES_UPDATE_CUBE_VERTICES = false;
glm::mat4 cube_rot_mat = glm::mat4(1);

void WindowManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void WindowManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			void* data = glfwGetWindowUserPointer(window);
			WindowManager* w = static_cast<WindowManager*>(data);

			w->mouse_button_pressed = true;
			glfwGetCursorPos(window, &w->old_x_pos, &w->old_y_pos);
			//current_scene_flag = (current_scene_flag + 1) % 2;
		}
		else if (action == GLFW_RELEASE)
		{
			void* data = glfwGetWindowUserPointer(window);
			WindowManager* w = static_cast<WindowManager*>(data);

			w->mouse_button_pressed = false;
		}
	}
}

void WindowManager::mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{

	WindowManager* w = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));

	if (!w->mouse_button_pressed)
		return;

	w->x_pos_diff = x_pos - w->old_x_pos;
	w->y_pos_diff = y_pos - w->old_y_pos;

	// TODO: if check not necessary, since mouse_button_callback is always called first
	//if (!(x_pos_diff == 0 && y_pos_diff == 0))
	{
		VLOG(4) << "old_x_pos = " << w->old_x_pos << ",\told_y_pos = " << w->old_y_pos;
		VLOG(4) << "x_pos = " << x_pos << ",\ty_pos = " << y_pos <<
			", x_pos_diff = " << w->x_pos_diff <<
			", y_pos_diff = " << w->y_pos_diff;
		VLOG(4) << "rotation axis = (" << w->y_pos_diff << ", "
			<< w->x_pos_diff << ")";

		OGL_EXAMPLES_UPDATE_CUBE_VERTICES = true;
		w->old_x_pos = x_pos;
		w->old_y_pos = y_pos;

		// double negation of y_pos_diff, because of y-axis inversion  
		cube_rot_mat = glm::rotate(glm::mat4(1), 0.03f,
			glm::vec3(w->y_pos_diff, w->x_pos_diff, 0.0));
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
		glfwWaitEventsTimeout(0.01); // medium CPU usage
		//glfwWaitEvents(); // almost no CPU usage

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

} // namespace ogl_examples