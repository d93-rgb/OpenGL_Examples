#pragma once
#include "opengl_examples.h"
#include "renderingmanager.h"
#include "windowmanager.h"

namespace ogl_examples
{

class RenderingDevice
{
public:
	RenderingDevice();
	~RenderingDevice();

	void change_renderer(const std::shared_ptr<Renderer> new_renderer);
	void run() const;

protected:
	std::unique_ptr<WindowManager> wm;
	std::shared_ptr<RenderingManager> rm;
};


} // namespace ogl_examples