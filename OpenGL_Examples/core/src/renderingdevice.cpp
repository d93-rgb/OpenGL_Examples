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

	rm->renderers.emplace(RENDERER_ENUM_LIST::RGB_CUBE, 
		std::make_shared<CubeRenderer>(
		gui_params,
		std::make_unique<CubeRendererEventHandler>(gui_params),
		std::make_shared<CubeRendererParameter>(wm.get())));
	
	rm->renderers.emplace(RENDERER_ENUM_LIST::RGB_TRIANGLE,
		std::make_shared<TriangleRenderer>(
			gui_params,
			std::make_unique<TriangleRendererEventHandler>(gui_params),
			std::make_shared<TriangleRendererParameter>(wm.get())));

	rm->renderers.emplace(RENDERER_ENUM_LIST::CIRCLE_SDF,
		std::make_shared<SDFRenderer>(
		gui_params,
		std::make_unique<TriangleRendererEventHandler>(gui_params),
		std::make_shared<TriangleRendererParameter>(wm.get())));

	rm->renderers.emplace(RENDERER_ENUM_LIST::FOURIER_SERIES,
		std::make_shared<FourierSeriesRenderer>(
			gui_params,
			std::make_unique<FourierSeriesRendererEventHandler>(gui_params),
			std::make_shared<FourierSeriesRendererParameter>(wm.get())));

	rm->change_renderer(rm->renderers.find(RENDERER_ENUM_LIST::FOURIER_SERIES)->second);
	
	wm->set_renderering_manager(rm);
}

RenderingDevice::~RenderingDevice() = default;

void RenderingDevice::run() const
{
	wm->run();
}


} // namespace ogl_examples