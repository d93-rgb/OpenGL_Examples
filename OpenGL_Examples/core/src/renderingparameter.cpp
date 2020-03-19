#include "renderingparameter.h"

namespace ogl_examples
{

RenderingParameter::RenderingParameter(const WindowManager* wm) :
	wm(wm)
{}

TriangleRendererParameter::TriangleRendererParameter(const WindowManager* wm) :
	RenderingParameter(wm)
{}

CubeRendererParameter::CubeRendererParameter(const WindowManager* wm) :
	RenderingParameter(wm)
{}

FourierSeriesRendererParameter::FourierSeriesRendererParameter(const WindowManager* wm) :
	RenderingParameter(wm)
{}

} // namespace ogl_examples