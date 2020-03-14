#include "renderingdevice.h"
#include "windowmanager.h"
#include "renderingmanager.h"
#include "renderingparameter.h"
#include "eventhandler.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice(const std::shared_ptr<GUIParameter>& gui_params) :
	rm(new RenderingManager(gui_params))
{
	GLuint screen_width = 800;
	GLuint screen_height = 600;

	wm.reset(new WindowManager(screen_width, screen_height));

	rm->renderers.push_back(std::make_shared<CubeRenderer>(
		gui_params,
		std::make_unique<CubeRendererEventHandler>(gui_params),
		std::make_shared<CubeRendererParameter>(wm.get())));
		
	rm->renderers.push_back(std::make_shared<BlueTriangleRenderer>(
		gui_params,
		std::make_unique<TriangleRendererEventHandler>(gui_params),
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