#pragma once
#include "opengl_examples.h"
#include "renderer.h"
#include "windowmanager.h"


namespace ogl_examples
{

class RenderingDevice
{
public:
	RenderingDevice();

	void run();

protected:
	std::unique_ptr<WindowManager> sc;
	std::unique_ptr<Renderer> sc;
};


} // namespace ogl_examples