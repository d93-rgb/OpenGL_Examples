#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class EventHandler
{
public:
	explicit EventHandler(std::shared_ptr<GUIParameter> gui_params);

	virtual void handle_mouse(GLFWwindow* window, double x_pos, double y_pos) = 0;

	virtual void handle_mouse_button(GLFWwindow* window, int button, int action, int mods) = 0;

	virtual void handle_framebuffer_size(GLFWwindow* window, int width, int height) = 0;

	virtual void handle_key(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;


protected:
	std::shared_ptr<GUIParameter> gui_params;
};

class TriangleRendererEventHandler : public EventHandler
{
public:
	explicit TriangleRendererEventHandler(std::shared_ptr<GUIParameter> gui_params);

	void handle_mouse(GLFWwindow* window, double x_pos, double y_pos);

	void handle_mouse_button(GLFWwindow* window, int button, int action, int mods);

	void handle_framebuffer_size(GLFWwindow* window, int width, int height);

	void handle_key(GLFWwindow* window, int key, int scancode, int action, int mode);
protected:

};

class CubeRendererEventHandler : public EventHandler
{
public:
	explicit CubeRendererEventHandler(std::shared_ptr<GUIParameter> gui_params);

	void handle_mouse(GLFWwindow* window, double x_pos, double y_pos);

	void handle_mouse_button(GLFWwindow* window, int button, int action, int mods);

	void handle_framebuffer_size(GLFWwindow* window, int width, int height);

	void handle_key(GLFWwindow* window, int key, int scancode, int action, int mode);

private:
	bool mouse_button_pressed;

	double old_x_pos;
	double old_y_pos;
	double new_x_pos;
	double new_y_pos;
	double x_pos_diff;
	double y_pos_diff;
};

class FourierSeriesRendererEventHandler : public EventHandler
{
public:
	explicit FourierSeriesRendererEventHandler(std::shared_ptr<GUIParameter> gui_params);

	void handle_mouse(GLFWwindow* window, double x_pos, double y_pos);

	void handle_mouse_button(GLFWwindow* window, int button, int action, int mods);

	void handle_framebuffer_size(GLFWwindow* window, int width, int height);

	void handle_key(GLFWwindow* window, int key, int scancode, int action, int mode);
private:
	bool mouse_button_pressed = false;

	double old_x_pos = 0;
	double old_y_pos = 0;
	double x_pos_diff = 0;
	double y_pos_diff = 0;
};


} // namespace ogl_examples