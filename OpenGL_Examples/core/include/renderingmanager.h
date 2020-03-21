#pragma once
#include "opengl_examples.h"
#include "renderer.h"

namespace ogl_examples
{

class RenderingManager
{
public:
	std::unordered_map<RENDERER_ENUM_LIST, std::shared_ptr<Renderer>> renderers;

	RenderingManager();

	explicit RenderingManager(std::shared_ptr<Renderer> renderer);

	explicit RenderingManager(std::shared_ptr<GUIParameter> gui_params);

	void run();

	void change_renderer(std::shared_ptr<Renderer> new_renderer);

	void recompile_shaders();

	void clean();

	const Renderer* get_current_renderer();

	GUIParameter* get_gui_params();

protected:
	std::shared_ptr<Renderer> current_renderer;
	std::shared_ptr<GUIParameter> gui_params;

};


} // namespace ogl_examples