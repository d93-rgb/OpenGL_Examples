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

	virtual void recompile() = 0;

	virtual void use_program();

protected:
	std::unique_ptr<ShaderCompiler> sc;
};

class TriangleRenderer : public Renderer
{
public:
	TriangleRenderer();

	void render();

	void recompile();

private:
	unsigned int VAO;
};

} // namespace ogl_examples