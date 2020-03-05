#include "renderingdevice.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice() :
	wm(new WindowManager(800, 600)),
	sc(new TriangleRenderer())
{
	wm->set_renderer(sc);
}

void RenderingDevice::run()
{
	wm->run();
}


} // namespace ogl_examples