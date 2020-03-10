#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class GUIParameter
{
public:
	GUIParameter();

	~GUIParameter();
protected:
	class CubeRendererParams
	{
		friend class CubeRenderer;

		bool update_cube_vertices;
		glm::mat4 cube_rot_mat;
	} cube_renderer_params;

};

} // namespace ogl_examples#pragma once
