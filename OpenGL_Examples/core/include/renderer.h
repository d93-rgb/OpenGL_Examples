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
};

class TriangleRenderer : public Renderer
{
public:
	TriangleRenderer::TriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render();

private:
	unsigned int VAO;
};

class BlueTriangleRenderer : public Renderer
{
public:
	BlueTriangleRenderer::BlueTriangleRenderer(const std::shared_ptr<TriangleRendererParameter>& render_params);

	void render();

private:
	unsigned int VAO;
};
} // namespace ogl_examples