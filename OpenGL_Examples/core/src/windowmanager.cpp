#include "renderingmanager.h"
#include "windowmanager.h"
#include "graphicscontext.h"
#include "eventhandler.h"
#include "guiparameter.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
	if (!w->io->WantCaptureMouse)
		w->rm->get_current_renderer()->eh->handle_mouse_button(window, button, action, mods);
}

void WindowManager::mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{

	WindowManager* w = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (!w->io->WantCaptureMouse)
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
	glfwSwapInterval(1); // vsync

	window = glfwCreateWindow(
		screen_width,
		screen_height,
		"OpenGL Example",
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

	// imgui initialization
		// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// TODO: raw pointer here that's used in static callback function, 
	//		 not feeling good about this at the moment
	io = &ImGui::GetIO();

	assert(io);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	gc.reset(new GraphicsContext(screen_width, screen_height));
}

void WindowManager::run()
{
	static float translation[] = { 0.0, 0.0 };
	static float color[4] = { 1.0f,1.0f,1.0f,1.0f };

	auto& gui_params = rm->get_current_renderer()->gui_params;
	float* t_floats = static_cast<float*>(&gui_params->cube_renderer_params.translation_vec.x);
	float* xy_rot_floats = static_cast<float*>(&gui_params->cube_renderer_params.rotation_xy.x);


	glfwShowWindow(window);
	int init = 0;
	while (!glfwWindowShouldClose(window)) {
		//glfwPollEvents(); // high CPU usage
		//glfwWaitEventsTimeout(0.01); // medium CPU usage
		glfwWaitEvents(); // low CPU usage

		rm->run();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render your GUI
		ImGui::Begin("Rotation / Position");
		static int i = 0;
		ImGui::Combo("test", &i, "a\0b\0c");
		gui_params->cube_renderer_params.rot_x_val_changed =
			ImGui::SliderFloat("x-rotation", &xy_rot_floats[0], 0, 2 * 3.1415);
		gui_params->cube_renderer_params.rot_y_val_changed =
			ImGui::SliderFloat("y-rotation", &xy_rot_floats[1], 0, 2 * 3.1415);
		gui_params->cube_renderer_params.trans_val_changed =
			ImGui::SliderFloat2("position", t_floats, -1.0, 1.0);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// TODO: limit calls to SwapBuffers for better gpu usage
		glfwSwapBuffers(window);
	}

	rm->clean();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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