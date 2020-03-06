#include "renderingmanager.h"
#include "graphicscontext.h"

namespace ogl_examples
{

RenderingManager::RenderingManager() :
	current_renderer(new TriangleRenderer(std::make_shared<TriangleRendererParameter>()))
{

}

RenderingManager::RenderingManager(const std::shared_ptr<Renderer> renderer) :
	current_renderer(renderer)
{

}

void RenderingManager::run()
{
	// TODO: inefficient, slow and uses globals - refactoring coming soon
	static bool i = true;
	if (current_scene_flag != old_scene_flag)
	{
		old_scene_flag = current_scene_flag;
		(i = !i) ? change_renderer(std::make_shared<TriangleRenderer>(std::make_shared<TriangleRendererParameter>())) :
			change_renderer(std::make_shared<BlueTriangleRenderer>(std::make_shared < TriangleRendererParameter>()));
	}

	current_renderer->use_program();
	current_renderer->render();
}


void RenderingManager::change_renderer(std::shared_ptr<Renderer> new_renderer)
{
	current_renderer = std::move(new_renderer);
}

void RenderingManager::recompile_shaders()
{
	current_renderer->recompile();
}

void RenderingManager::add_renderer(std::shared_ptr<Renderer> renderer)
{
	renderers.push_back(std::move(renderer));
}

} // namespace ogl_examples