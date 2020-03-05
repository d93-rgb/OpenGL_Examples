#pragma once
#include "opengl_examples.h"
#include "renderer.h"

namespace ogl_examples
{

class RenderingManager
{
public:
	RenderingManager();

	void run() const;

	void change_renderer(const std::shared_ptr<Renderer>& new_renderer);

protected:
	std::shared_ptr<Renderer> current_renderer;

};


} // namespace ogl_examples