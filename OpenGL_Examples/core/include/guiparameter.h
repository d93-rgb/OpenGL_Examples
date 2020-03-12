#pragma once
#include "opengl_examples.h"
#include "imgui.h"

namespace ogl_examples
{

class GUIParameter
{
public:
	GUIParameter();

	~GUIParameter();

	class CubeRendererParams
	{
	public:
		CubeRendererParams()
		{}
	private:
		friend class WindowManager;
		friend class CubeRendererEventHandler;
		friend class CubeRenderer;

		bool update_cube_vertices = false;
		bool rot_x_val_changed = false;
		bool rot_y_val_changed = false;
		bool trans_val_changed = false;

		glm::mat4 cube_rot_mat{ 1 };
		glm::vec2 translation_vec{ 0 };
		glm::vec2 rotation_xy{ 0 };
	} cube_renderer_params;

};

} // namespace ogl_examples#pragma once
