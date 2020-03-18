#include "guiparameter.h"

namespace ogl_examples
{

GUIParameter::GUIParameter()
{
	renderer_names.push_back("RGB Cube");
	renderer_names.push_back("RGB Triangle");
	renderer_names.push_back("Circle SDF");
	renderer_names.push_back("Fourier Series");

	int i = -1;
	choice_to_scene_map.emplace(++i, RENDERER_ENUM_LIST::RGB_CUBE);
	scene_to_choice_map.emplace(RENDERER_ENUM_LIST::RGB_CUBE, i);

	choice_to_scene_map.emplace(++i, RENDERER_ENUM_LIST::RGB_TRIANGLE);
	scene_to_choice_map.emplace(RENDERER_ENUM_LIST::RGB_TRIANGLE, i);

	choice_to_scene_map.emplace(++i, RENDERER_ENUM_LIST::CIRCLE_SDF);
	scene_to_choice_map.emplace(RENDERER_ENUM_LIST::CIRCLE_SDF, i);

	choice_to_scene_map.emplace(++i, RENDERER_ENUM_LIST::FOURIER_SERIES);
	scene_to_choice_map.emplace(RENDERER_ENUM_LIST::FOURIER_SERIES, i);

	scene_choice = scene_to_choice_map.find(RENDERER_ENUM_LIST::FOURIER_SERIES)->second;
}

GUIParameter::~GUIParameter() = default;

} // namespace ogl_examples