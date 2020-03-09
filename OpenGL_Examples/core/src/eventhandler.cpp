#pragma once
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "eventhandler.h"
#include "renderer.h"


namespace ogl_examples
{

CubeRendererEventHandler::CubeRendererEventHandler() :
	mouse_button_pressed(false),
	old_x_pos(0),
	old_y_pos(0),
	new_x_pos(0),
	new_y_pos(0),
	x_pos_diff(0),
	y_pos_diff(0)
{

}

void CubeRendererEventHandler::set_renderer(std::shared_ptr<CubeRenderer> cr)
{
	this->cr = std::move(cr);
}

void CubeRendererEventHandler::handle_mouse(GLFWwindow* window, double x_pos, double y_pos)
{
	if (!mouse_button_pressed)
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

		cr->update_cube_vertices = true;
		old_x_pos = x_pos;
		old_y_pos = y_pos;

		// double negation of y_pos_diff, because of y-axis inversion  
		cr->cube_rot_mat = glm::rotate(glm::mat4(1), 0.03f,
			glm::vec3(y_pos_diff, x_pos_diff, 0.0));
	}
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

} // namespace ogl_examples