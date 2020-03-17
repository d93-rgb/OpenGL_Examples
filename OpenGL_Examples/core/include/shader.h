#pragma once
#include "opengl_examples.h"
#include "GL/gl3w.h"

namespace ogl_examples
{

class Shader
{
public:
    Shader();

    ~Shader();

protected:
    struct Uniform
    {
        int location;

        template <typename T>
        Uniform(
            const std::string& uniform_name,
            GLint location,
            const T& val,
            GLsizei n) :
            location(location)
        {
            set_uniform(val, n);
        }

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

    std::unordered_map<std::string, Uniform> uniforms;
};

} // namespace ogl_examples
