#include "guiparameter.h"

namespace ogl_examples
{

GUIParameter::GUIParameter()
{
	renderer_names.push_back("RGB Cube");
	renderer_names.push_back("RGB Triangle");
	renderer_names.push_back("Circle SDF");
	renderer_names.push_back("Fourier Series");

	int i = 0;
	scene_map.emplace(i++, RENDERER_ENUM_LIST::RGB_CUBE);
	scene_map.emplace(i++, RENDERER_ENUM_LIST::RGB_TRIANGLE);
	scene_map.emplace(i++, RENDERER_ENUM_LIST::CIRCLE_SDF);
	scene_map.emplace(i++, RENDERER_ENUM_LIST::FOURIER_SERIES);
}

GUIParameter::~GUIParameter() = default;

} // namespace ogl_examples
