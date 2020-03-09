#include "renderingdevice.h"
#include "renderingparameter.h"
#include "eventhandler.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice() :
	rm(new RenderingManager())
{
	GLuint screen_width = 800;
	GLuint screen_height = 600;

	wm.reset(new WindowManager(screen_width, screen_height));

	rm->renderers.push_back(std::make_shared<CubeRenderer>(
		std::make_unique<CubeRendererEventHandler>(),
		std::make_shared<CubeRendererParameter>(wm.get())));
		
	dynamic_cast<CubeRendererEventHandler*>(rm->renderers.back()->eh.get())->
		set_renderer(dynamic_cast<CubeRenderer*>(rm->renderers.back().get()));

	rm->renderers.push_back(std::make_shared<BlueTriangleRenderer>(
		std::make_unique<TriangleRendererEventHandler>(),
		std::make_shared<TriangleRendererParameter>(wm.get())));
	rm->change_renderer(rm->renderers.front());
	
	wm->set_renderering_manager(rm);
}

RenderingDevice::~RenderingDevice() = default;

void RenderingDevice::run() const
{
	wm->run();
}


} // namespace ogl_examples