#pragma once
#include "opengl_examples.h"

#include <GL/gl3w.h>

namespace ogl_examples
{

class ShaderCompiler
{
public:
	ShaderCompiler(const std::string& vertex_src, const std::string& fragment_src);

	void create_program();

private:
	void compile(const std::string& type);
	void check_errors(GLuint shader, const std::string& type) const;
	void link();

	unsigned int vertex, fragment;
	std::string vertexPath;
	std::string fragPath;

	GLint program_id;
};

} // namespace ogl_examples