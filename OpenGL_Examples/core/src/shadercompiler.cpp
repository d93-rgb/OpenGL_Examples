#include <fstream>
#include <sstream>
#include <string>

#include "opengl_examples.h"
#include "shadercompiler.h"

namespace ogl_examples
{

ShaderCompiler::ShaderCompiler(const std::string& vertex_src, const std::string& fragment_src) :
vertexPath(vertex_src), fragPath(fragment_src), vertex(0), fragment(0)
{
	create_program();
}


void ShaderCompiler::compile(const std::string& type)
{
	const std::string* src;
	if (type == "vertex")
	{
		src = &vertexPath;
	}
	else if (type == "fragment")
	{
		src = &fragPath;
	}
	else
	{
		LOG(ERROR) << "error: expected shader type: vertex, fragment. received shader type:"
			<< type;
		std::exit(1);
	}

	std::ifstream shaderFile;
	std::string shaderCode;

	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		shaderFile.open(*src);
		std::stringstream shaderStream;
		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		LOG(ERROR) << "error: shader file not successfully read" << std::endl;
		std::exit(1);
	}

	auto shaderCode_raw = shaderCode.c_str();
	if (type == "vertex")
	{
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &shaderCode_raw, NULL);
		glCompileShader(vertex);
		check_errors(vertex, "VERTEX");
	}
	else if (type == "fragment")
	{
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &shaderCode_raw, NULL);
		glCompileShader(fragment);
		check_errors(fragment, "FRAGMENT");
	}
}

void ShaderCompiler::link()
{
	// shader Program
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex);
	glAttachShader(program_id, fragment);
	glLinkProgram(program_id);
	check_errors(program_id, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ShaderCompiler::create_program()
{
	compile("vertex");
	compile("fragment");
	link(); 
}

GLuint ShaderCompiler::get_program_id()
{
	return program_id;
}

void ShaderCompiler::check_errors(GLuint shader, const std::string& type) const
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG(ERROR) << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "/n" << infoLog
				<< "/n -- --------------------------------------------------- -- ";
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG(ERROR) << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "/n" << infoLog
				<< "/n -- --------------------------------------------------- -- ";
		}
	}
}

} // namespace ogl_examples