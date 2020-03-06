#include "renderingdevice.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice() :
	wm(new WindowManager(800, 600)),
	rm(new RenderingManager())
{

	rm->renderers.push_back(std::make_shared<TriangleRenderer>(std::make_shared<TriangleRendererParameter>()));
	rm->renderers.push_back(std::make_shared<BlueTriangleRenderer>(std::make_shared<TriangleRendererParameter>()));
	rm->change_renderer(rm->renderers.front());
	
	wm->set_renderering_manager(rm);
}

void RenderingDevice::run() const
{
	wm->run();
}


} // namespace ogl_examples