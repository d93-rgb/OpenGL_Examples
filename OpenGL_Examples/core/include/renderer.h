#pragma once
#include "GL/gl3w.h"

#include "opengl_examples.h"
#include "shadercompiler.h"
#include "windowmanager.h"

namespace ogl_examples
{

class Renderer
{
public:
	Renderer();

	virtual void render();

protected:
	std::unique_ptr<ShaderCompiler> sc;
};

class TriangleRenderer : public Renderer
{
	TriangleRenderer();

	void render();

private:
	unsigned int VAO;
};

} // namespace ogl_examples