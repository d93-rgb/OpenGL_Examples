#include "renderingmanager.h"

namespace ogl_examples
{

RenderingManager::RenderingManager() :
	current_renderer(new TriangleRenderer())
{

}

RenderingManager::RenderingManager(const std::shared_ptr<Renderer> renderer) :
	current_renderer(renderer)
{

}

void RenderingManager::run() const
{
	current_renderer->use_program();
	current_renderer->render();
}

void RenderingManager::change_renderer(const std::shared_ptr<Renderer>& new_renderer)
{
	current_renderer = new_renderer;
}

void RenderingManager::recompile_shaders()
{
	current_renderer->recompile();
}

} // namespace ogl_examples