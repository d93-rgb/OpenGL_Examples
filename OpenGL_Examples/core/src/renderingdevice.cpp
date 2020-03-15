#include "renderingdevice.h"
#include "windowmanager.h"
#include "renderingmanager.h"
#include "renderingparameter.h"
#include "eventhandler.h"
#include "guiparameter.h"

namespace ogl_examples
{

RenderingDevice::RenderingDevice(const std::shared_ptr<GUIParameter>& gui_params) :
	rm(new RenderingManager(gui_params))
{
	wm.reset(new WindowManager(gui_params->screen_width, gui_params->screen_height));

	int renderer_count = 0;
	rm->renderers.emplace(renderer_count++, 
		std::make_shared<CubeRenderer>(
		gui_params,
		std::make_unique<CubeRendererEventHandler>(gui_params),
		std::make_shared<CubeRendererParameter>(wm.get())));
	
	rm->renderers.emplace(renderer_count++,
		std::make_shared<TriangleRenderer>(
			gui_params,
			std::make_unique<TriangleRendererEventHandler>(gui_params),
			std::make_shared<TriangleRendererParameter>(wm.get())));

	rm->renderers.emplace(renderer_count++,
		std::make_shared<SDFRenderer>(
		gui_params,
		std::make_unique<TriangleRendererEventHandler>(gui_params),
		std::make_shared<TriangleRendererParameter>(wm.get())));

	rm->change_renderer(rm->renderers.find(0)->second);
	
	wm->set_renderering_manager(rm);
}

RenderingDevice::~RenderingDevice() = default;

void RenderingDevice::run() const
{
	wm->run();
}


} // namespace ogl_examples