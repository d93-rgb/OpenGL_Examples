#include "windowmanager.h"
#include "renderer.h"
#include "camera.h"
#include "renderingparameter.h"
#include "glm/gtc/matrix_transform.hpp"


namespace ogl_examples
{

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

TriangleRenderer::TriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params)
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

BlueTriangleRenderer::BlueTriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params)
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

CubeRenderer::CubeRenderer(const std::shared_ptr<CubeRendererParameter>& render_params) :
	render_params(render_params)
{
	std::string vertexPath = "../../../../OpenGL_Examples/Rendering-Cube/src/shaders/vertex_shader.glsl";
	std::string fragPath = "../../../../OpenGL_Examples/Rendering-Cube/src/shaders/fragment_shader.glsl";

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	struct cube {
		float vertices[24] = {
		-0.5, -0.5, -0.5,
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
		-0.5, -0.5,  0.5,
		 0.5, -0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		 0.5, -0.5, -0.5
		};

		float colors[24] = {
			1.0, 1.0, 1.0,
			0.0, 1.0, 0.0,
			1.0, 1.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 1.0,
			0.0, 0.0, 0.0,
			0.0, 1.0, 1.0,
			0.0, 0.0, 1.0
		};
	} cube;

	unsigned int cube_indices[] = {
		0, 2, 1,
		0, 3, 2,
		0, 1, 6,
		0, 6, 7,
		1, 5, 6,
		1, 2, 5,
		5, 2, 3,
		5, 3, 4,
		5, 7, 6,
		5, 4, 7,
		4, 0, 7,
		4, 3, 0
	};

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)24);

	// unbind everything
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cam.reset(new PerspectiveCamera(
		glm::lookAt(glm::vec3(2.0, 0.0, 0.0), 
		glm::vec3(0.0), 
		glm::vec3(0.0, 1.0, 0.0)), 
		glm::radians(90.0), 
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
	float angle = glfwGetTime();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	uniforms.back().set_uniform(glm::rotate(glm::mat4(1), angle, glm::vec3(0.0, 1.0, 0.0)), 1);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}

} // namespace ogl_examples