#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class RenderingDevice
{
public:
	explicit RenderingDevice(const std::shared_ptr<GUIParameter>& gui_params);
	~RenderingDevice();

	void change_renderer(const std::shared_ptr<Renderer> new_renderer);
	void run() const;

protected:
	std::shared_ptr<WindowManager> wm;
	std::shared_ptr<RenderingManager> rm;
};


} // namespace ogl_examples