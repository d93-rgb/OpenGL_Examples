#pragma once
#include "GL/gl3w.h"

#include "opengl_examples.h"
#include "shadercompiler.h"
#include "renderingparameter.h"

namespace ogl_examples
{

class Renderer
{
public:

	virtual void render() = 0;

	virtual void recompile();

	virtual void use_program();

protected:

	std::unique_ptr<ShaderCompiler> sc;

private:
	struct Uniform
	{
		int location;

		Uniform(int location) :
			location(location) {}

		void set_uniform(const int* val, GLsizei n)
		{
			glUniform1iv(location, n, val);
		}

		void set_uniform(const unsigned int* val, GLsizei n)
		{
			glUniform1uiv(location, n, val);
		}

		void set_uniform(const float* val, GLsizei n)
		{
			glUniform1fv(location, n, val);
		}

        void set_uniform(const glm::ivec2* val, GLsizei  n)
        {
            glUniform2iv(location, n, (const int*)val);
        }

        void set_uniform(const glm::ivec3* val, GLsizei  n)
        {
            glUniform3iv(location, n, (const int*)val);
        }

        void set_uniform(const glm::vec2* val, GLsizei  n)
        {
            glUniform2fv(location, n, (const float*)val);
        }

        void set_uniform(const glm::vec3* val, GLsizei  n)
        {
            glUniform3fv(location, n, (const float*)val);
        }

        void set_uniform(const glm::vec4* val, GLsizei  n)
        {
            glUniform4fv(location, n, (const float*)val);
        }

        void set_uniform(const glm::mat2* mat, GLsizei  n)
        {
            glUniformMatrix2fv(location, n, 0, (const float*)mat);
        }

        void set_uniform(const glm::mat3* mat, GLsizei  n)
        {
            glUniformMatrix3fv(location, n, 0, (const float*)mat);
        }

        void set_uniform(const glm::mat4* mat, GLsizei  n)
        {
            glUniformMatrix4fv(location, n, 0, (const float*)mat);
        }
	};

    std::vector<Uniform> uniforms;
};

class TriangleRenderer : public Renderer
{
public:
	TriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render();

private:
	unsigned int VAO;
};

class BlueTriangleRenderer : public Renderer
{
public:
	BlueTriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render();

private:
	unsigned int VAO;
};
} // namespace ogl_examples