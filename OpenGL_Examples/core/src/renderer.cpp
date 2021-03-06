#include "shader.h"
#include "camera.h"
#include "windowmanager.h"
#include "renderer.h"
#include "camera.h"
#include "renderingparameter.h"
#include "eventhandler.h"
#include "guiparameter.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"


namespace ogl_examples
{

Renderer::Renderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh) :
	gui_params(std::move(gui_params)),
	eh(std::move(eh))
{

}

Renderer::~Renderer() = default;

Shader& Renderer::use_shader(const std::string& shader_name)
{
	return shaders.find(shader_name)->second.use_program();
}

void Renderer::recompile()
{
	for (auto& s : shaders)
	{
		s.second.recompile();
	}
}

TriangleRenderer::TriangleRenderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh,
	const std::shared_ptr<TriangleRendererParameter>& render_params) :
	Renderer(std::move(gui_params), std::move(eh))
{
	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-Triangle/src/shaders/vertex_shader.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-Triangle/src/shaders/fragment_shader.glsl";

	shaders.emplace(triangle_shader_name, Shader(vertexPath, fragPath));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindVertexArray(0);
}

void TriangleRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	use_shader(triangle_shader_name);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}

void TriangleRenderer::clean()
{
	glDeleteVertexArrays(1, &VAO);
}

SDFRenderer::SDFRenderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh,
	const std::shared_ptr<TriangleRendererParameter>& render_params) :
	Renderer(std::move(gui_params), std::move(eh))
{
	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-SDF/src/shaders/vertex_shader.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-SDF/src/shaders/fragment_shader.glsl";

	shaders.emplace(circle_sdf_shader_name, Shader(vertexPath, fragPath));

	use_shader(circle_sdf_shader_name).create_uniform(
		"resolution",
		glm::vec2(this->gui_params->screen_width, this->gui_params->screen_height),
		1);

	glUseProgram(0);

	glGenVertexArrays(1, &VAO);
}

void SDFRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	use_shader(circle_sdf_shader_name);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
}

void SDFRenderer::clean()
{
	glDeleteVertexArrays(1, &VAO);
}

CubeRenderer::CubeRenderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh,
	const std::shared_ptr<CubeRendererParameter>& render_params) :
	Renderer(std::move(gui_params), std::move(eh)),
	render_params(render_params)
{
	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-Cube/src/shaders/vertex_shader.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-Cube/src/shaders/fragment_shader.glsl";

	shaders.emplace(cube_shader_name, Shader(vertexPath, fragPath));

	// TODO: no shape base class done yet, this is a replacement for objToWorld matrix,
	//		 moving the cube once at initialization and not during the rendering loop
	for (int i = 0; i < 8; ++i) {
		cube.vertices[i] =
			glm::rotate(glm::radians(45.0f), glm::vec3(0, -1, 0)) *
			glm::rotate(glm::radians(30.0f), glm::vec3(1, 0, 0)) *
			glm::vec4(cube.vertices[i], 1);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(cube.vertices)));

	// unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// TODO: move into shape class, cube not seen if ebo is set to 0
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cam.reset(new PerspectiveCamera(
		gui_params,
		glm::lookAt(glm::vec3(0.0, 0.0, 4.0),
			glm::vec3(0.0),
			glm::vec3(0.0, 1.0, 0.0)),
		glm::radians(45.0),
		static_cast<float>(this->gui_params->screen_width) / this->gui_params->screen_height,
		0.1,
		100));

	auto m = glm::mat4(1);
	auto t_vec = glm::vec4(0);

	use_shader(cube_shader_name).
		create_uniform("worldToRaster",
			cam->worldToRaster,
			1).
		create_uniform("objToWorld_rot_x",
			m,
			1).
		create_uniform("objToWorld_rot_y",
			m,
			1).
		create_uniform("trans_vec",
			t_vec,
			1);

	glUseProgram(0);
}

void CubeRenderer::render()
{
	use_shader(cube_shader_name);
	glBindVertexArray(VAO);

	if (gui_params->cube_renderer_params.trans_val_changed)
	{
		use_shader(cube_shader_name).set_uniform(
			"trans_vec",
			glm::vec4(gui_params->cube_renderer_params.translation_vec, 0, 0),
			1);
	}

	if (update_vertices())
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

bool CubeRenderer::update_vertices()
{
	static constexpr float angle_rad = 0.03f;
	static float old_rot_x_val = 0;
	static float old_rot_y_val = 0;
	//static float diff_val;

	if (gui_params->cube_renderer_params.rot_x_val_changed)
	{
		/*uniforms.find("objToWorld_rot_x")->second.set_uniform(
			glm::rotate(glm::mat4(1),
				gui_params->cube_renderer_params.rotation_xy.x,
				glm::vec3(1, 0, 0)),
			1);*/

			//diff_val = gui_params->cube_renderer_params.rotation_xy.x - old_rot_x_val;

		for (int i = 0; i < 8; ++i)
		{
			cube.vertices[i] = glm::rotateX(cube.vertices[i],
				gui_params->cube_renderer_params.rotation_xy.x - old_rot_x_val);
		}
		old_rot_x_val = gui_params->cube_renderer_params.rotation_xy.x;
	}

	if (gui_params->cube_renderer_params.rot_y_val_changed)
	{
		/*uniforms.find("objToWorld_rot_y")->second.set_uniform(
			glm::rotate(glm::mat4(1),
				gui_params->cube_renderer_params.rotation_xy.y,
				glm::vec3(0, 1, 0)),
			1);*/
			//diff_val = gui_params->cube_renderer_params.rotation_xy.y - old_rot_y_val;

		for (int i = 0; i < 8; ++i)
		{
			cube.vertices[i] = glm::rotateY(cube.vertices[i],
				gui_params->cube_renderer_params.rotation_xy.y - old_rot_y_val);
		}
		old_rot_y_val = gui_params->cube_renderer_params.rotation_xy.y;

	}

	return rotate_vertices_mouse() ||
		gui_params->cube_renderer_params.rot_x_val_changed ||
		gui_params->cube_renderer_params.rot_y_val_changed;
}

bool CubeRenderer::rotate_vertices_mouse()
{
	if (gui_params->cube_renderer_params.update_cube_vertices)
	{
		gui_params->cube_renderer_params.update_cube_vertices = false;
		gui_params->cube_renderer_params.rotation_xy = glm::vec2(0);

		for (int i = 0; i < 8; ++i)
		{
			cube.vertices[i] = gui_params->cube_renderer_params.cube_rot_mat
				* glm::vec4(cube.vertices[i], 1);
		}
		return true;
	}
	return false;
}

void CubeRenderer::clean()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

FourierSeriesRenderer::FourierSeriesRenderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh,
	const std::shared_ptr<FourierSeriesRendererParameter>& render_params) :
	Renderer(std::move(gui_params), std::move(eh)),
	render_params(render_params)
{
	std::string ring_vertexPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/ring_vs.glsl";
	std::string ring_fragPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/ring_fs.glsl";

	std::string vector_vertexPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/vector_vs.glsl";
	std::string vector_fragPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/vector_fs.glsl";

	std::string tracer_vertexPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/tracer_vs.glsl";
	std::string tracer_fragPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/tracer_fs.glsl";


	shaders.emplace(ring_shader_name, Shader(ring_vertexPath, ring_fragPath));
	shaders.emplace(vector_shader_name, Shader(vector_vertexPath, vector_fragPath));
	shaders.emplace(tracer_shader_name, Shader(tracer_vertexPath, tracer_fragPath));

	float period = 1.0f;
	auto rectangle_func = [&](float t) -> std::complex<float> {
		if (t < period * 0.5)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}
	};

	{
		std::vector<float> real_coefficients;
		std::vector<float> a_k;
		std::vector<float> b_k;

		int highest_coeff_k = 5;
		int sample_count = 1000;
		fill_func_values(rectangle_func, period, sample_count);
		fill_fourier_coeff(highest_coeff_k);

		std::vector<int> coeff_indices;
		coeff_indices.push_back(0);
		a_k.push_back(fourier_coefficients[0].real() * 2.0f);
		b_k.push_back(0);
		real_coefficients.push_back(a_k.back());

		for (int i = 1, j = 1; i <= (fourier_coefficients.size() - 1) / 2; ++i, j += 2)
		{
			coeff_indices.push_back(i);
			coeff_indices.push_back(-i);

			a_k.push_back((fourier_coefficients[j] + fourier_coefficients[j + 1]).real());
			b_k.push_back(-(fourier_coefficients[j] - fourier_coefficients[j + 1]).imag());

			float abs_val = sqrtf(a_k.back() * a_k.back() + b_k.back() * b_k.back());
			real_coefficients.push_back(abs_val);
		}

		for (int i = 0; i < real_coefficients.size(); ++i)
		{
			//std::complex cplx =
			//	std::complex(0.5f * cosf(i * M_PI / 3), 0.5f * sinf(i * M_PI / 3));
			Ring r(
				//abs(fourier_coefficients[i]),
				real_coefficients[i],
				this->gui_params->fourierseries_renderer_params.ring_thickness,
				this->gui_params->fourierseries_renderer_params.ring_vertices);

			Vector v{
				//fourier_coefficients[i],
				//coeff_indices[i] * static_cast<float>(TWO_PI) / period,
				real_coefficients[i] *
				std::complex<float>(cosf(atan2f(a_k[i], b_k[i])), sinf(atan2f(a_k[i], b_k[i]))),
				i * static_cast<float>(TWO_PI) * inv_function_period,
				this->gui_params->fourierseries_renderer_params.vector_line_height,
				this->gui_params->fourierseries_renderer_params.vector_arrow_base_width };

			vr_pairs.push_back(VectorRingPair(std::move(v), std::move(r)));
		}
	}

	float ar = static_cast<float>(this->gui_params->screen_width) / this->gui_params->screen_height;
	cam.reset(new OrthographicCamera(
		this->gui_params,
		glm::lookAt(glm::vec3(0.0, 0.0, 1.0),
			glm::vec3(0.0),
			glm::vec3(0.0, 1.0, 0.0)),
		// TODO: change view window, add options for runtime changing
		4.0f,
		ar,
		0.1,
		100,
		&(this->gui_params->fourierseries_renderer_params.camera_zoom),
		&this->gui_params->fourierseries_renderer_params.camera_translation));

	use_shader(ring_shader_name).
		create_uniform("worldToRaster",
			cam->worldToRaster,
			1).
		create_uniform("objToWorld",
			glm::mat4(1),
			1).
		create_uniform("color",
			this->gui_params->fourierseries_renderer_params.ring_color,
			1);

	use_shader(vector_shader_name).
		create_uniform("worldToRaster",
			cam->worldToRaster,
			1).
		create_uniform("objToWorld",
			glm::mat4(1),
			1).
		create_uniform("color",
			this->gui_params->fourierseries_renderer_params.vector_color,
			1);

	use_shader(tracer_shader_name).
		create_uniform("worldToRaster",
			cam->worldToRaster,
			1).
		create_uniform("objToWorld",
			glm::mat4(1),
			1).
		create_uniform("color",
			glm::vec4(1),
			1);

	glUseProgram(0);
}


// TODO: instanced drawing
void FourierSeriesRenderer::render()
{
	static Shader* current_shader;
	static bool thickness_greater_than_radius = true;
	static float time_delta = 0.0f;

	if (!gui_params->fourierseries_renderer_params.animate)
	{
		return;
	}

	std::vector<glm::mat4> translations;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gui_params->fourierseries_renderer_params.update_camera)
	{
		gui_params->fourierseries_renderer_params.update_camera = false;
		cam->update();
	}

	current_shader = &use_shader(vector_shader_name);

	if (gui_params->fourierseries_renderer_params.update_vector_colors)
	{
		gui_params->fourierseries_renderer_params.update_vector_colors = false;
		current_shader->set_uniform("color",
			gui_params->fourierseries_renderer_params.vector_color, 1);
	}

	/*if (gui_params->fourierseries_renderer_params.update_vectors ||
		gui_params->fourierseries_renderer_params.update_rings)
	{
		thickness_greater_than_radius =
			gui_params->fourierseries_renderer_params.ring_thickness <
			gui_params->fourierseries_renderer_params.ring_radius;

		if (thickness_greater_than_radius)
		{
			vr_pairs[0].v = Vector(
				gui_params->fourierseries_renderer_params.ring_radius -
				gui_params->fourierseries_renderer_params.ring_thickness,
				gui_params->fourierseries_renderer_params.vector_line_height,
				gui_params->fourierseries_renderer_params.vector_arrow_base_width);

			for (size_t i = 1; i < vr_pairs.size(); ++i)
			{
				vr_pairs[i].v = Vector(
					gui_params->fourierseries_renderer_params.ring_radius -
					gui_params->fourierseries_renderer_params.ring_thickness,
					gui_params->fourierseries_renderer_params.vector_line_height,
					gui_params->fourierseries_renderer_params.vector_arrow_base_width);
			}
		}
	}*/

	float time = glfwGetTime() - gui_params->fourierseries_renderer_params.resume_delta;
	auto rot_mat = glm::mat4(1);
	current_shader->
		set_uniform("objToWorld",
			rot_mat,
			1).
		set_uniform("worldToRaster",
			cam->worldToRaster, 1);

	vr_pairs[0].v.draw();

	translations.push_back(
		glm::translate(
			glm::vec3(rot_mat * glm::vec4(*vr_pairs[0].v.arrow_tip, 0, 1))));

	for (size_t i = 1; i < vr_pairs.size(); ++i)
	{
		rot_mat = glm::rotate(time * vr_pairs[i].v.angular_velocity * 0.1f, glm::vec3(0, 0, 1));

		current_shader->set_uniform("objToWorld",
			translations.back() * rot_mat,
			1);

		translations.push_back(
			translations.back() *
			glm::translate(
				glm::vec3(rot_mat * glm::vec4(*vr_pairs[i].v.arrow_tip, 0, 1))));

		vr_pairs[i].v.draw();
	}

	current_shader = &use_shader(ring_shader_name);

	if (gui_params->fourierseries_renderer_params.update_ring_colors)
	{
		gui_params->fourierseries_renderer_params.update_ring_colors = false;
		current_shader->set_uniform("color",
			gui_params->fourierseries_renderer_params.ring_color, 1);
	}

	if (gui_params->fourierseries_renderer_params.update_vectors ||
		gui_params->fourierseries_renderer_params.update_rings)
	{
		gui_params->fourierseries_renderer_params.update_vectors = false;
		gui_params->fourierseries_renderer_params.update_rings = false;

		//if (thickness_greater_than_radius)
		{
			vr_pairs[0].r = Ring(vr_pairs[0].v.vector_magnitude,
				gui_params->fourierseries_renderer_params.ring_thickness,
				gui_params->fourierseries_renderer_params.ring_vertices);

			for (size_t i = 1; i < vr_pairs.size(); ++i)
			{
				vr_pairs[i].r = Ring(vr_pairs[i].v.vector_magnitude,
					gui_params->fourierseries_renderer_params.ring_thickness,
					gui_params->fourierseries_renderer_params.ring_vertices);
			}
		}
	}

	current_shader->
		set_uniform("objToWorld",
			glm::mat4(1),
			1).
		set_uniform("worldToRaster",
			cam->worldToRaster, 1);

	vr_pairs[0].r.draw();

	for (size_t i = 1; i < vr_pairs.size(); ++i)
	{
		current_shader->set_uniform("objToWorld",
			translations[i - 1],
			1);

		vr_pairs[i].r.draw();
	}

	/*
	*
	* Vector tip tracer
	*
	*/
	current_shader = &use_shader(tracer_shader_name);

	// draw on top
	glClear(GL_DEPTH_BUFFER_BIT);

	// TODO: make this more object oriented and substitute glBindBuffer calls
	// with glBindSubBuffer etc..
	static const float dist_eps = 1e-7f;
	static const size_t max_points = 1000;

	static std::vector<glm::vec2> trace_points;
	static std::vector<GLuint> trace_indices(2 * max_points);
	static glm::vec2 old_tip_pos = glm::vec2(translations.back()[3]);
	static float distance;
	static int trace_idx = 0;
	static size_t trace_start_index = 0;
	static GLuint trace_vao;
	static GLuint trace_vbo;
	static GLuint trace_ebo;
	static bool init = true;

	if (init)
	{
		init = false;

		trace_points.reserve(max_points);

		glGenVertexArrays(1, &trace_vao);
		glGenBuffers(1, &trace_vbo);
		glGenBuffers(1, &trace_ebo);

		for (int i = 0; i < max_points; ++i)
		{
			trace_indices[i] = i;
			trace_indices[max_points + i] = i;
		}
	}

	// update current "drawing" tip position
	auto new_tip_pos = glm::vec2(translations.back()[3]);
	distance = glm::length(new_tip_pos - old_tip_pos);
	old_tip_pos = new_tip_pos;

	glBindVertexArray(trace_vao);
	if (distance > dist_eps)
	{
		if (trace_points.size() >= max_points)
		{
			++trace_start_index;
			if (trace_start_index >= max_points)
			{
				trace_start_index = 0;
			}

			trace_points[trace_idx++] = old_tip_pos;
			if (trace_idx >= max_points)
			{
				trace_idx = 0;
			}
		}
		else
		{
			trace_points.push_back(old_tip_pos);
		}

		glBindBuffer(GL_ARRAY_BUFFER, trace_vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			trace_points.size() * sizeof(trace_points.front()),
			&trace_points[0],
			GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trace_ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			trace_points.size() * sizeof(trace_indices.front()),
			&trace_indices[trace_start_index],
			GL_STATIC_DRAW);

		// vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	}

	current_shader->set_uniform("worldToRaster",
		cam->worldToRaster, 1);

	glDrawElements(GL_LINE_STRIP, trace_points.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void FourierSeriesRenderer::clean()
{
	for (auto& tmp : vr_pairs)
	{
		tmp.r.clean();
		tmp.v.clean();
	}
}

FourierSeriesRenderer::Ring::Ring(float radius, float thickness, int n) :
	radius(abs(radius))
{
	if (!n)
	{
		LOG(INFO) << "n must be greater than 0";
		std::exit(1);
	}
	if (this->radius < thickness)
	{
		thickness = 0.9 * radius;
		//LOG(ERROR) << "thickness cannot be greater than radius";
		//std::exit(1);
	}

	for (int i = 0; i < n; ++i)
	{
		vertices.push_back(glm::vec2(
			this->radius * cos(i * TWO_PI / n),
			this->radius * sin(i * TWO_PI / n)
			));
	}

	for (int i = 0; i < n; ++i)
	{
		vertices.push_back(glm::vec2(
		(this->radius - thickness) * static_cast<float>(cos(i * TWO_PI / n)),
			(this->radius - thickness) * static_cast<float>(sin(i * TWO_PI / n))
			));
	}

	for (int i = 0; i < n - 1; ++i)
	{
		// first triangle
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + n);

		// second triangle
		indices.push_back(i + 1);
		indices.push_back(i + n + 1);
		indices.push_back(i + n);
	}
	// last triangle cases handled separately to avoid modulo operations
	// first triangle
	indices.push_back(n - 1);
	indices.push_back(0);
	indices.push_back(n - 1 + n);

	// second triangle
	indices.push_back(0);
	indices.push_back(n);
	indices.push_back(n - 1 + n);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vertices.front()),
		&vertices[0],
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(indices.front()),
		&indices[0],
		GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// TODO: same as for cube renderer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void FourierSeriesRenderer::Ring::draw()
{
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void FourierSeriesRenderer::Ring::clean()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

FourierSeriesRenderer::Line::Line(float width, float height)
{
	float height_half = height * 0.5f;

	vertices.push_back(glm::vec2(0, height_half));
	vertices.push_back(glm::vec2(0, -height_half));
	vertices.push_back(glm::vec2(width, height_half));
	vertices.push_back(glm::vec2(width, -height_half));

	// first triangle
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	// second triangle
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
}

FourierSeriesRenderer::Arrow::Arrow(float base_width, float height)
{
	float base_width_half = base_width * 0.5f;

	vertices.push_back(glm::vec2(0, base_width_half));
	vertices.push_back(glm::vec2(0, -base_width_half));

	arrow_tip = glm::vec2(height, 0);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
}

FourierSeriesRenderer::Vector::Vector(
	std::complex<float> cplx,
	float angular_velocity,
	float line_height,
	float arrow_base_width) :
	angular_velocity(angular_velocity)
{
	//assert(vector_length <= 1 && vector_length >= 0);

	vector_magnitude = std::abs(cplx);
	float scale_factor = 2 * vector_magnitude;
	//float new_arrow_length = arrow_length * (abs_cplx < 0.5f ? sqrt(2) * sqrt(abs_cplx) : 1.0f);
	float new_arrow_length = arrow_length * (vector_magnitude < 0.5f ? scale_factor : 1.0f);
	float line_length = vector_magnitude - new_arrow_length;

	Line line(line_length, scale_factor * line_height);
	Arrow arrow(arrow_base_width, new_arrow_length);

	vertices = line.vertices;
	indices = line.indices;

	for (auto& v : arrow.vertices)
	{
		if (vector_magnitude < 0.5f)
		{
			//v = glm::scale(glm::vec3(sqrt(2) * sqrt(abs_cplx), sqrt(2) * sqrt(abs_cplx), 0)) * glm::vec4(v, 0, 0);
			v = glm::scale(glm::vec3(scale_factor, scale_factor, 0)) * glm::vec4(v, 0, 0);
		}

		v = glm::vec2(line_length, 0) + v;
	}
	arrow.arrow_tip += glm::vec2(line_length, 0);


	for (auto& i : arrow.indices)
	{
		i += vertices.size();
	}

	vertices.insert(vertices.end(),
		arrow.vertices.begin(),
		arrow.vertices.end());
	vertices.push_back(arrow.arrow_tip);
	arrow_tip = &vertices.back();

	// rotate to starting position
	for (auto& v : vertices)
	{
		v = glm::rotateZ(glm::vec3(v, 0), std::arg(cplx));
	}

	indices.insert(indices.end(),
		arrow.indices.begin(),
		arrow.indices.end());

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vertices.front()),
		&vertices[0],
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(indices.front()),
		&indices[0],
		GL_DYNAMIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// TODO: same as for cube renderer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void FourierSeriesRenderer::Vector::draw()
{
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void FourierSeriesRenderer::Vector::clean()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

std::complex<float> FourierSeriesRenderer::integrate(int k)
{
	std::complex<float> exp_term;
	std::complex<float> c_k = 0;

	// average value, skip cos and sin calls
	if (k == 0)
	{
		for (int i = 0; i < function_data.size(); ++i)
		{
			c_k += function_data[i].second * dx;
		}

		return c_k * inv_function_period;
	}

	for (int i = 0; i < function_data.size(); ++i)
	{
		exp_term = std::complex<float>(
			cosf(TWO_PI * inv_function_period * k * function_data[i].first),
			-sinf(TWO_PI * inv_function_period * k * function_data[i].first));

		c_k += function_data[i].second * exp_term * dx;
	}

	return c_k * inv_function_period;
}

void FourierSeriesRenderer::fill_fourier_coeff(
	int n) // highest coefficient index
{
	// return if period is less than or equal to 0
	if (function_period < FLT_EPSILON)
	{
		return;
	}

	// average value
	fourier_coefficients.push_back(integrate(0));

	std::complex<float> coeff_tmp;
	for (int i = 1; i <= n; ++i)
	{
		coeff_tmp = integrate(i);
		fourier_coefficients.push_back(coeff_tmp);
		coeff_tmp = integrate(-i);
		fourier_coefficients.push_back(coeff_tmp);
	}
}

void FourierSeriesRenderer::fill_func_values(
	const std::function<std::complex<float>(float)>& func,
	float period,
	int n) // sample point count
{
	function_data.resize(n);
	function_period = period;
	inv_function_period = 1.0f / period;

	dx = period / n;
	float x = 0;
	for (int i = 0; i < n; ++i)
	{
		x = static_cast<float>(i) * dx;
		function_data[i] = { x, func(x) };
	}
}

} // namespace ogl_examples