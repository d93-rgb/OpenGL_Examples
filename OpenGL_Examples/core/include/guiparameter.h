#pragma once
#include "GL/gl3w.h"

#include "opengl_examples.h"
#include "imgui.h"

namespace ogl_examples
{
enum class RENDERER_ENUM_LIST
{
	RGB_CUBE, RGB_TRIANGLE, CIRCLE_SDF, FOURIER_SERIES
};

class GUIParameter
{
public:
	GUIParameter();

	~GUIParameter();

	GLuint screen_width = 1080;
	GLuint screen_height = 800;

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

	class FourierSeriesParams
	{
	public:
		FourierSeriesParams()
		{}

	private:
		friend class WindowManager;
		friend class FourierSeriesEventHandler;
		friend class FourierSeriesRenderer;
		friend class OrthographicCamera;

		bool update_rings = false;
		bool update_vectors = false;

		bool update_ring_colors = false;
		bool update_vector_colors = false;
		
		bool update_camera = false;

		int ring_vertices = 15;
		float ring_radius = 0.7f;
		float ring_thickness = 0.012f;

		float vector_length = 0.5;
		float vector_line_height = 0.012;
		float vector_arrow_base_width = 0.120;

		float camera_zoom = 1.0f;

		glm::vec4 ring_color = glm::vec4(0.4, 0.4, 0.4, 0.3);
		glm::vec4 vector_color = glm::vec4(0.2, 0.3, 0.8, 1.0);

	} fourierseries_renderer_params;

	bool switch_renderer = false;
	int scene_choice;

	std::vector<std::string> renderer_names;
	std::unordered_map<int, RENDERER_ENUM_LIST> choice_to_scene_map;
	std::unordered_map<RENDERER_ENUM_LIST, int> scene_to_choice_map;
};

} // namespace ogl_examples#pragma once
