#pragma once
#include <unordered_map>
#include <complex>

#include "GL/gl3w.h"

#include "opengl_examples.h"
#include "shape.h"
#include "shader.h"

namespace ogl_examples
{

class Renderer
{
public:
	std::unique_ptr<EventHandler> eh;
	std::shared_ptr<GUIParameter> gui_params;

	Renderer(std::shared_ptr<GUIParameter> gui_params, std::unique_ptr<EventHandler> eh);
	~Renderer();

	virtual void render() = 0;

	virtual void recompile();

	virtual Shader& use_shader(const std::string& shader_name);

    virtual void clean() = 0;

protected:

    std::unique_ptr<Camera> cam;
	std::unordered_map<std::string, Shader> shaders;
};

class TriangleRenderer : public Renderer
{
public:
	TriangleRenderer(
		std::shared_ptr<GUIParameter> gui_params, 
		std::unique_ptr<EventHandler> eh,
		const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render() override;
    void clean() override;


private:
	unsigned int VAO;

	std::string triangle_shader_name = "triangle";
};

class SDFRenderer : public Renderer
{
public:
	SDFRenderer(
		std::shared_ptr<GUIParameter> gui_params,
		std::unique_ptr<EventHandler> eh,
		const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render() override;
    void clean() override;

private:
	unsigned int VAO;

	std::string circle_sdf_shader_name = "circle_sdf";
};

class CubeRenderer : public Renderer
{
public:
    std::shared_ptr<CubeRendererParameter> render_params;

	CubeRenderer(const std::shared_ptr<CubeRendererParameter>& render_params);

    CubeRenderer(
		std::shared_ptr<GUIParameter> gui_params,
		std::unique_ptr<EventHandler> eh, 
		const std::shared_ptr<CubeRendererParameter>& render_params);

    void render() override;
	bool update_vertices();
	bool rotate_vertices_mouse();
    void clean() override;

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

	std::string cube_shader_name = "cube";

	struct cube {
		glm::vec3 vertices[8] = {
		glm::vec3(-0.5, -0.5, -0.5),
		glm::vec3(-0.5,  0.5, -0.5),
		glm::vec3(-0.5,  0.5,  0.5),
		glm::vec3(-0.5, -0.5,  0.5),
		glm::vec3( 0.5, -0.5,  0.5),
		glm::vec3( 0.5,  0.5,  0.5),
		glm::vec3( 0.5,  0.5, -0.5),
		glm::vec3( 0.5, -0.5, -0.5)
		};

		float colors[24] = {
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			1.0, 1.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 1.0,
			1.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 0.0, 1.0
		};
	} cube;

	unsigned int cube_indices[36] = {
		0, 2, 1,
		0, 3, 2,
		0, 1, 6,
		0, 6, 7,
		1, 6, 5,
		1, 5, 2,
		5, 2, 3,
		5, 3, 4,
		5, 7, 6,
		5, 4, 7,
		4, 0, 7,
		4, 3, 0
	};

};

class FourierSeriesRenderer : public Renderer
{
public:
	std::shared_ptr<FourierSeriesRendererParameter> render_params;

	FourierSeriesRenderer(
		std::shared_ptr<GUIParameter> gui_params,
		std::unique_ptr<EventHandler> eh,
		const std::shared_ptr<FourierSeriesRendererParameter>& render_params);

	void render() override;
	void clean() override;

private:
	void fourier_coeff(int n);
	void update_vectors();

	struct Ring : public Shape
	{
		Ring(float radius, float thickness, int n);

		float radius;
		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;

		glm::mat4 objToWorld;

		void draw() override;
		void clean() override;

		Ring operator+(const glm::vec2& vec)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			for (auto& v : vertices)
				v += vec;
			glBufferData(
				GL_ARRAY_BUFFER,
				vertices.size() * sizeof(vertices.front()),
				&vertices[0],
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return *this;
		}

	private:
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;
	};

	struct Line
	{
		Line(float width, float height);

		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;
	};

	struct Arrow
	{
		Arrow(float base_width, float height);

		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;

		glm::vec2 arrow_tip;
	};

	struct Vector : public Shape
	{
		Vector(
			std::complex<float> cplx,
			float line_height,
			float arrow_base_width); // vertical base side of triangle

		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;
		
		glm::vec2* arrow_tip;

		glm::mat4 objToWorld;

		void draw() override;
		void clean() override;

		Vector operator+(const glm::vec2& vec)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			for (auto& v : vertices)
				v += vec;
			glBufferData(
				GL_ARRAY_BUFFER,
				vertices.size() * sizeof(vertices.front()),
				&vertices[0],
				GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return *this;
		}

	private:
		float arrow_length = 0.145; // hardcoded value that makes arrow look nice
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;
	};
	
	struct VectorRingPair
	{
		VectorRingPair(Vector v, Ring r) :
			v(std::move(v)), r(std::move(r))
		{}
		
		Vector v;
		Ring r;
	};

	std::string ring_shader_name = "ring";
	std::string vector_shader_name = "vector";
	std::string tracer_shader_name = "tracer";

	std::vector<VectorRingPair> vr_pairs;

	std::vector<std::pair<float, std::complex<float>>> function_data;
	std::vector<std::complex<float>> fourier_coefficients;
};

} // namespace ogl_examples