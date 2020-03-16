#include "shadercompiler.h"
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

void Renderer::use_program()
{
	sc->use_program();
}

void Renderer::recompile()
{
	sc->create_program();
}

template<typename T>
inline void Renderer::create_uniform(const std::string& uniform_name, int location, const T& val, GLsizei n)
{
	uniforms.emplace(uniform_name, Uniform(uniform_name, location, val, n));
}

TriangleRenderer::TriangleRenderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh,
	const std::shared_ptr<TriangleRendererParameter>& render_params) :
	Renderer(std::move(gui_params), std::move(eh))
{
	//std::string file_path = std::string(__FILE__);
	//file_path = file_path.substr(0, file_path.find_last_of("\\/"));

	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-Triangle/src/shaders/vertex_shader.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-Triangle/src/shaders/fragment_shader.glsl";

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindVertexArray(0);
}

void TriangleRenderer::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	sc->use_program();

	auto uniform_name = "resolution";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		glm::vec2(this->gui_params->screen_width, this->gui_params->screen_height),
		1);

	glUseProgram(0);

	glGenVertexArrays(1, &VAO);
}

void SDFRenderer::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	// TODO: no shape base class done yet, this is a replacement for objToWorld matrix,
	//		 moving the cube once at initialization and not during the rendering loop
	for (int i = 0; i < 8; ++i) {
		cube.vertices[i] =
			glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, -1, 0)) *
			glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(1, 0, 0)) *
			glm::vec4(cube.vertices[i], 1);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	
	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(cube.vertices)));

	// unbind everything
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cam.reset(new PerspectiveCamera(
		glm::lookAt(glm::vec3(0.0, 0.0, 4.0),
			glm::vec3(0.0),
			glm::vec3(0.0, 1.0, 0.0)),
		glm::radians(45.0),
		static_cast<float>(this->gui_params->screen_width) / this->gui_params->screen_height,
		0.1,
		100));

	auto m = glm::mat4(1);
	auto t_vec = glm::vec4(0);

	sc->use_program();

	auto uniform_name = "worldToRaster";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		cam->worldToRaster,
		1);

	uniform_name = "objToWorld_rot_x";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		m,
		1);

	uniform_name = "objToWorld_rot_y";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		m,
		1);

	uniform_name = "trans_vec";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		t_vec,
		1);

	glUseProgram(0);
}

void CubeRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);

	if (gui_params->cube_renderer_params.trans_val_changed)
	{
		uniforms.find("trans_vec")->second.set_uniform(
			(glm::vec4(gui_params->cube_renderer_params.translation_vec, 0, 0)), (size_t)1);
	}

	if (update_vertices())
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/vertex_shader.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-FourierSeries/src/shaders/fragment_shader.glsl";

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	create_ring(
		this->gui_params->fourierseries_renderer_params.radius,
		this->gui_params->fourierseries_renderer_params.thickness,
		this->gui_params->fourierseries_renderer_params.vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, 
		vr_pairs.front().r.vertices.size() * sizeof(vr_pairs.front().r.vertices.front()), 
		&vr_pairs.front().r.vertices[0], 
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		vr_pairs.front().r.indices.size() * sizeof(vr_pairs.front().r.indices.front()),
		&vr_pairs.front().r.indices[0], 
		GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// colors
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(cube.vertices)));

	// unbind everything
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	sc->use_program();

	float ar = static_cast<float>(this->gui_params->screen_width) / this->gui_params->screen_height;
	cam.reset(new OrthographicCamera(
		glm::lookAt(glm::vec3(0.0, 0.0, 1.0),
			glm::vec3(0.0),
			glm::vec3(0.0, 1.0, 0.0)),
		-1.0 * ar,
		1.0 * ar,
		1.0,
		-1.0,
		0.1,
		100));

	sc->use_program();

	auto uniform_name = "worldToRaster";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		cam->worldToRaster,
		1);

	uniform_name = "color";
	create_uniform(uniform_name,
		glGetUniformLocation(sc->get_program_id(), uniform_name),
		this->gui_params->fourierseries_renderer_params.circle_color,
		1);

	glUseProgram(0);
}

void FourierSeriesRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);


	if(gui_params->fourierseries_renderer_params.update_circle)
	{
		gui_params->fourierseries_renderer_params.update_circle = false;

		create_ring(gui_params->fourierseries_renderer_params.radius,
			gui_params->fourierseries_renderer_params.thickness,
			gui_params->fourierseries_renderer_params.vertices);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			vr_pairs.back().r.vertices.size() * sizeof(vr_pairs.back().r.vertices.front()),
			&vr_pairs.back().r.vertices[0],
			GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			vr_pairs.back().r.indices.size() * sizeof(vr_pairs.back().r.indices.front()),
			&vr_pairs.back().r.indices[0],
			GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (gui_params->fourierseries_renderer_params.update_circle_color)
	{
		gui_params->fourierseries_renderer_params.update_circle = false;
		uniforms.find("color")->second.set_uniform(
			gui_params->fourierseries_renderer_params.circle_color, 1);
	}

	glDrawElements(GL_TRIANGLES, vr_pairs.back().r.indices.size(), GL_UNSIGNED_INT, 0);
}

void FourierSeriesRenderer::clean()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void FourierSeriesRenderer::create_ring(float radius, float thickness, int n)
{
	if (!n)
	{
		LOG(INFO) << "n must be greater than 0";
		std::exit(1);
	}
	if (radius < thickness)
	{
		LOG(ERROR) << "thickness cannot be greater than radius";
		//std::exit(1);
		return;
	}

	Ring r;

	for (int i = 0; i < n; ++i)
	{
		r.vertices.push_back(glm::vec2(
			radius * cos(i * TWO_PI / n),
			radius * sin(i * TWO_PI / n)
		));
	}

	for (int i = 0; i < n; ++i)
	{
		r.vertices.push_back(glm::vec2(
			(radius - thickness) * static_cast<float>(cos(i * TWO_PI / n)),
			(radius - thickness) * static_cast<float>(sin(i * TWO_PI / n))
		));
	}

	for (int i = 0; i < n - 1; ++i)
	{
		// first triangle
		r.indices.push_back(i);
		r.indices.push_back(i + 1);
		r.indices.push_back(i + n);

		// second triangle
		r.indices.push_back(i + 1);
		r.indices.push_back(i + n + 1);
		r.indices.push_back(i + n);
	}
	// last triangle cases handled separately to avoid modulo operations
	// first triangle
	r.indices.push_back(n - 1);
	r.indices.push_back(0);
	r.indices.push_back(n - 1 + n);

	// second triangle
	r.indices.push_back(0);
	r.indices.push_back(n);
	r.indices.push_back(n - 1 + n);

	VectorRingPair vr_pair;
	vr_pair.r = std::move(r);

	// TODO: quick fix for having only one element inside container, either add new 
	//		 method or change to something more elegant
	if(!vr_pairs.empty())
		vr_pairs.pop_back();

	vr_pairs.push_back(std::move(vr_pair));
}

FourierSeriesRenderer::Line FourierSeriesRenderer::create_line(float width, float height)
{
	Line line;

	float width_half = width * 0.5f;

	line.vertices.push_back(glm::vec2(-width_half, 0));
	line.vertices.push_back(glm::vec2(width_half, 0));
	line.vertices.push_back(glm::vec2(-width_half, height));
	line.vertices.push_back(glm::vec2(width_half, height));
	
	// first triangle
	line.indices.push_back(0);
	line.indices.push_back(1);
	line.indices.push_back(2);
	
	// second triangle
	line.indices.push_back(2);
	line.indices.push_back(1);
	line.indices.push_back(3);

	return line;
}

FourierSeriesRenderer::Arrow FourierSeriesRenderer::create_arrow(float base_width, float height)
{
	Arrow arrow;

	float base_width_half = base_width * 0.5f;

	arrow.vertices.push_back(glm::vec2(-base_width_half, 0));
	arrow.vertices.push_back(glm::vec2(base_width_half, 0));
	arrow.vertices.push_back(glm::vec2(0, height));

	// first triangle
	arrow.indices.push_back(0);
	arrow.indices.push_back(1);
	arrow.indices.push_back(2);
	
	return arrow;
}
} // namespace ogl_examples