#include "renderingdevice.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice() :
	rm(new RenderingManager())
{
	GLuint screen_width = 800;
	GLuint screen_height = 600;

	wm.reset(new WindowManager(screen_width, screen_height));

	rm->renderers.push_back(std::make_shared<CubeRenderer>(std::make_shared<CubeRendererParameter>(wm.get())));
	rm->renderers.push_back(std::make_shared<BlueTriangleRenderer>(std::make_shared<TriangleRendererParameter>(wm.get())));
	rm->change_renderer(rm->renderers.front());
	
	wm->set_renderering_manager(rm);
}

void RenderingDevice::run() const
{
	wm->run();
}


} // namespace ogl_examples