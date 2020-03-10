#include "shadercompiler.h"
#include "camera.h"
#include "windowmanager.h"
#include "renderer.h"
#include "camera.h"
#include "renderingparameter.h"
#include "eventhandler.h"
#include "guiparameter.h"

#include "glm/gtc/matrix_transform.hpp"


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
inline void Renderer::create_uniform(int location, const T* val, GLsizei n)
{
	uniforms.push_back(Uniform(location, val, n));
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

BlueTriangleRenderer::BlueTriangleRenderer(
	std::shared_ptr<GUIParameter> gui_params,
	std::unique_ptr<EventHandler> eh,
	const std::shared_ptr<TriangleRendererParameter>& render_params) :
	Renderer(std::move(gui_params), std::move(eh))
{
	//std::string file_path = std::string(__FILE__);
	//file_path = file_path.substr(0, file_path.find_last_of("\\/"));

	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-Triangle/src/shaders/vertex_shader2.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-Triangle/src/shaders/fragment_shader.glsl";

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindVertexArray(0);
}

void BlueTriangleRenderer::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}

void BlueTriangleRenderer::clean()
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
		render_params->wm->screen_width / render_params->wm->screen_height,
		0.1,
		100));

	auto m = glm::mat4(1);
	sc->use_program();
	uniforms.push_back(Uniform(glGetUniformLocation(sc->get_program_id(), "worldToRaster")));
	//uniforms.back().set_uniform(cam->worldToRaster, 1);
	uniforms.back().set_uniform(cam->worldToRaster, 1);

	uniforms.push_back(Uniform(glGetUniformLocation(sc->get_program_id(), "objToWorld")));
	uniforms.back().set_uniform(m, 1);

	uniforms.push_back(Uniform(glGetUniformLocation(sc->get_program_id(), "rotationMat")));
	uniforms.back().set_uniform(m, 1);

	glUseProgram(0);
}

void CubeRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);

	//float angle = glfwGetTime();
	if (gui_params->cube_renderer_params.update_cube_vertices)
	{
		gui_params->cube_renderer_params.update_cube_vertices = false;
		for (int i = 0; i < 8; ++i) {
			cube.vertices[i] = gui_params->cube_renderer_params.cube_rot_mat 
				* glm::vec4(cube.vertices[i], 1);
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void CubeRenderer::clean()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

} // namespace ogl_examples