#pragma once
#include <vector>

#include "opengl_examples.h"
#include "renderer.h"

namespace ogl_examples
{

class RenderingManager
{
public:
	RenderingManager();

	RenderingManager(const std::shared_ptr<Renderer> renderer);

	void run();

	void change_renderer(std::shared_ptr<Renderer> new_renderer);

	void recompile_shaders();

	void add_renderer(std::shared_ptr<Renderer> renderer);

protected:
	std::shared_ptr<Renderer> current_renderer;
	std::vector<std::shared_ptr<Renderer>> renderers;

};


} // namespace ogl_examples