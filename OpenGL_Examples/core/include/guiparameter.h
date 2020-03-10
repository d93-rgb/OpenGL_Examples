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
		CubeRendererParams() :
			update_cube_vertices(false),
			cube_rot_mat(1)
		{}
	private:
		friend class CubeRendererEventHandler;
		friend class CubeRenderer;

		bool update_cube_vertices;
		glm::mat4 cube_rot_mat;
	} cube_renderer_params;

};

} // namespace ogl_examples#pragma once
