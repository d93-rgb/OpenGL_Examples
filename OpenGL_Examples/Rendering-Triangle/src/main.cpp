#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "opengl_examples.h"
#include "shadercompiler.h"
#include "windowmanager.h"

int main(void)
{
	std::string file_path = std::string(__FILE__);
	file_path = file_path.substr(0, file_path.find_last_of("\\/"));

	std::string vertexPath = file_path + "/shaders/vertex_shader.glsl";
	std::string fragPath = file_path + "/shaders/fragment_shader.glsl";

	ogl_examples::ShaderCompiler shader_comp(vertexPath, fragPath);

	glEnable(GL_DEPTH_TEST);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_comp.use_program();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
