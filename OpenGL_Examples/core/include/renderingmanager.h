#pragma once
#include <vector>

#include "opengl_examples.h"
#include "renderer.h"

namespace ogl_examples
{

class RenderingManager
{
public:
	std::vector<std::shared_ptr<Renderer>> renderers;

	RenderingManager();

	RenderingManager(const std::shared_ptr<Renderer> renderer);

	void run();

	void change_renderer(std::shared_ptr<Renderer> new_renderer);

	void recompile_shaders();

	void clean();

protected:
	std::shared_ptr<Renderer> current_renderer;
};


} // namespace ogl_examples