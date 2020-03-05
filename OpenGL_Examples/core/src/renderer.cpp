#include "renderer.h"

namespace ogl_examples
{

TriangleRenderer::TriangleRenderer()
{
	std::string file_path = std::string(__FILE__);
	file_path = file_path.substr(0, file_path.find_last_of("\\/"));

	std::string vertexPath = file_path + "/shaders/vertex_shader.glsl";
	std::string fragPath = file_path + "/shaders/fragment_shader.glsl";

	this->sc.reset(new ShaderCompiler(vertexPath, fragPath));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void TriangleRenderer::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

}

} // namespace ogl_examples