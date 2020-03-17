#pragma once
#include <unordered_map>

#include "GL/gl3w.h"

#include "opengl_examples.h"

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

	void create_ring(float radius, float thickness, int n);
	
private:
	struct Line;
	struct Arrow;
	struct Vector;

	Line create_line(float width, float height);
	Arrow create_arrow(float base_width, float height);
	Vector create_vector();

	struct Ring
	{
		std::vector<glm::vec2> vertices;

		std::vector<unsigned int> indices;
	};

	struct Line
	{
		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;

	};

	struct Arrow
	{
		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;
	};

	struct Vector
	{
		std::vector<glm::vec2> vertices;
		std::vector<unsigned int> indices;

		void draw();
	};
	
	struct VectorRingPair
	{
		Vector v;
		Ring r;
	};

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	std::vector<VectorRingPair> vr_pairs;
};

} // namespace ogl_examples