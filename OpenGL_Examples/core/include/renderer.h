#pragma once
#include "GL/gl3w.h"

#include "opengl_examples.h"
#include "shadercompiler.h"
#include "renderingparameter.h"
#include "camera.h"

namespace ogl_examples
{

class Renderer
{
public:

	virtual void render() = 0;

	virtual void recompile();

	virtual void use_program();

    virtual void clean() = 0;

    template<typename T>
    void create_uniform(int location, const T* val, GLsizei n);

protected:

	struct Uniform
	{
		int location;

		Uniform(int location) :
			location(location) {}

		void set_uniform(const int& val, GLsizei n)
		{
			glUniform1iv(location, n, &val);
		}

		void set_uniform(const unsigned int& val, GLsizei n)
		{
			glUniform1uiv(location, n, &val);
		}

		void set_uniform(const float& val, GLsizei n)
		{
			glUniform1fv(location, n, &val);
		}

        void set_uniform(const glm::ivec2& val, GLsizei  n)
        {
            glUniform2iv(location, n, &(const int&)val);
        }

        void set_uniform(const glm::ivec3& val, GLsizei  n)
        {
            glUniform3iv(location, n, &(const int&)val);
        }

        void set_uniform(const glm::vec2& val, GLsizei  n)
        {
            glUniform2fv(location, n, &(const float&)val);
        }

        void set_uniform(const glm::vec3& val, GLsizei  n)
        {
            glUniform3fv(location, n, &(const float&)val);
        }

        void set_uniform(const glm::vec4& val, GLsizei  n)
        {
            glUniform4fv(location, n, &(const float&)val);
        }

        void set_uniform(const glm::mat2& mat, GLsizei  n)
        {
            glUniformMatrix2fv(location, n, 0, &(const float&)mat);
        }

        void set_uniform(const glm::mat3& mat, GLsizei  n)
        {
            glUniformMatrix3fv(location, n, 0, &(const float&)mat);
        }

        void set_uniform(const glm::mat4& mat, GLsizei  n)
        {
            glUniformMatrix4fv(location, n, 0, &(const float&)mat);
        }
	};

    std::unique_ptr<ShaderCompiler> sc;
    std::unique_ptr<Camera> cam;
    std::vector<Uniform> uniforms;
};

class TriangleRenderer : public Renderer
{
public:
	TriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render();
    void clean();


private:
	unsigned int VAO;
};

class BlueTriangleRenderer : public Renderer
{
public:
	BlueTriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render();
    void clean();

private:
	unsigned int VAO;
};

class CubeRenderer : public Renderer
{
public:
    std::shared_ptr<CubeRendererParameter> render_params;

    CubeRenderer(const std::shared_ptr<CubeRendererParameter>& render_params);

    void render();
    void clean();


private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

};

} // namespace ogl_examples