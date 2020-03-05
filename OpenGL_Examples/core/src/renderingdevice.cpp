#include "renderingdevice.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice() :
	wm(new WindowManager(800, 600)),
	rm(new RenderingManager())
{
	wm->set_renderering_manager(rm);
}

void RenderingDevice::run() const
{
	wm->run();
}


} // namespace ogl_examples