#pragma once
#include "opengl_examples.h"
#include "guiparameter.h"

namespace ogl_examples
{

GUIParameter::GUIParameter()
{
	int i = 0;
	scene_map.emplace(i++, "RGB Cube");
	scene_map.emplace(i++, "RGB Triangle");
	scene_map.emplace(i++, "Circle SDF");
	scene_map.emplace(i++, "Fourier Series");
}

GUIParameter::~GUIParameter() = default;

} // namespace ogl_examples
