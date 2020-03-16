#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "eventhandler.h"
#include "renderer.h"
#include "guiparameter.h"


namespace ogl_examples
{
EventHandler::EventHandler(std::shared_ptr<GUIParameter> gui_params) :
	gui_params(std::move(gui_params))
{

}

TriangleRendererEventHandler::TriangleRendererEventHandler(std::shared_ptr<GUIParameter> gui_params) :
	EventHandler(std::move(gui_params))
{

}

void TriangleRendererEventHandler::handle_mouse(GLFWwindow* window, double x_pos, double y_pos)
{

}

void TriangleRendererEventHandler::handle_mouse_button(GLFWwindow* window, int button, int action, int mods)
{

}

void TriangleRendererEventHandler::handle_framebuffer_size(GLFWwindow* window, int width, int height)
{

}

void TriangleRendererEventHandler::handle_key(GLFWwindow* window, int key, int scancode, int action, int mode)
{

}

CubeRendererEventHandler::CubeRendererEventHandler(std::shared_ptr<GUIParameter> gui_params) :
	EventHandler(std::move(gui_params)),
	mouse_button_pressed(false),
	old_x_pos(0),
	old_y_pos(0),
	new_x_pos(0),
	new_y_pos(0),
	x_pos_diff(0),
	y_pos_diff(0)
{

}

void CubeRendererEventHandler::handle_mouse(GLFWwindow* window, double x_pos, double y_pos)
{
	if (!mouse_button_pressed || (x_pos == old_x_pos && y_pos == old_y_pos))
		return;

	x_pos_diff = x_pos - old_x_pos;
	y_pos_diff = y_pos - old_y_pos;

	// TODO: if check not necessary, since mouse_button_callback is always called first
	//if (!(x_pos_diff == 0 && y_pos_diff == 0))
	{
		VLOG(4) << "old_x_pos = " << old_x_pos << ",\told_y_pos = " << old_y_pos;
		VLOG(4) << "x_pos = " << x_pos << ",\ty_pos = " << y_pos <<
			", x_pos_diff = " << x_pos_diff <<
			", y_pos_diff = " << y_pos_diff;
		VLOG(4) << "rotation axis = (" << y_pos_diff << ", "
			<< x_pos_diff << ")";

		gui_params->cube_renderer_params.update_cube_vertices = true;
		old_x_pos = x_pos;
		old_y_pos = y_pos;

		// double negation of y_pos_diff, because of y-axis inversion  
		gui_params->cube_renderer_params.cube_rot_mat = glm::rotate(glm::mat4(1), 0.03f,
			glm::vec3(y_pos_diff, x_pos_diff, 0.0));
	}
}

void CubeRendererEventHandler::handle_framebuffer_size(GLFWwindow* window, int width, int height)
{

}

void CubeRendererEventHandler::handle_mouse_button(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouse_button_pressed = true;
			glfwGetCursorPos(window, &old_x_pos, &old_y_pos);
			//current_scene_flag = (current_scene_flag + 1) % 2;
		}
		else if (action == GLFW_RELEASE)
		{
			mouse_button_pressed = false;
		}
	}
}

void CubeRendererEventHandler::handle_key(GLFWwindow* window, int key, int scancode, int action, int mode)
{

}

FourierSeriesRendererEventHandler::FourierSeriesRendererEventHandler(std::shared_ptr<GUIParameter> gui_params) :
	EventHandler(std::move(gui_params))
{

}

void FourierSeriesRendererEventHandler::handle_mouse(GLFWwindow* window, double x_pos, double y_pos)
{

}

void FourierSeriesRendererEventHandler::handle_mouse_button(GLFWwindow* window, int button, int action, int mods)
{

}

void FourierSeriesRendererEventHandler::handle_framebuffer_size(GLFWwindow* window, int width, int height)
{

}

void FourierSeriesRendererEventHandler::handle_key(GLFWwindow* window, int key, int scancode, int action, int mode)
{

}

} // namespace ogl_examples