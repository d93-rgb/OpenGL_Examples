#include "renderingmanager.h"
#include "graphicscontext.h"
#include "guiparameter.h"

namespace ogl_examples
{

RenderingManager::RenderingManager()
{
}

RenderingManager::RenderingManager(std::shared_ptr<Renderer> renderer) :
	current_renderer(renderer)
{
}

RenderingManager::RenderingManager(std::shared_ptr<GUIParameter> gui_params) :
	gui_params(gui_params)
{
}

void RenderingManager::run()
{

	if (gui_params->switch_renderer)
	{
		current_renderer = renderers.find(gui_params->scene_choice)->second;
	}

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

void RenderingManager::clean()
{
	for (const auto& r : renderers)
	{
		r.second->clean();
	}
}

const Renderer* RenderingManager::get_current_renderer()
{
	return current_renderer.get();
}

GUIParameter* RenderingManager::get_gui_params()
{
	return gui_params.get();
}

} // namespace ogl_examples