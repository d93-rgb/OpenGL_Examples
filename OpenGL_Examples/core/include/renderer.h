#pragma once
#include "GL/gl3w.h"

#include "opengl_examples.h"
#include "shadercompiler.h"

namespace ogl_examples
{

class Renderer
{
public:

	virtual void render() = 0;

protected:
	std::unique_ptr<ShaderCompiler> sc;
};

class TriangleRenderer : public Renderer
{
public:
	TriangleRenderer();

	void render();

private:
	unsigned int VAO;
};

} // namespace ogl_examples