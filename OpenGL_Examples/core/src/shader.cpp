#include "opengl_examples.h"
#include "shader.h"
#include "shadercompiler.h"

namespace ogl_examples
{

Shader::Shader(const std::string& vertex_src, const std::string& fragment_src) :
	sc(vertex_src, fragment_src)
{

}

Shader::~Shader() = default;

template<typename T>
inline Shader& Shader::create_uniform(const std::string& uniform_name, const T& val, GLsizei n)
{
	uniforms.emplace(uniform_name,
		Uniform(
			uniform_name,
			glGetUniformLocation(program_id, uniform_name),
			val,
			n));

	return *this;
}

template<typename T>
inline Shader& Shader::set_uniform(const std::string& uniform_name, const T& val, GLsizei n)
{
	uniforms.find(uniform_name)->second.set_uniform(val,n);

	return *this;
}

Shader& Shader::use_program()
{
	assert(glIsProgram(program_id)); // should never happen normally
	glUseProgram(program_id);

	return *this;
}

} // namespace ogl_examples
