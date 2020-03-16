#pragma once
#include "opengl_examples.h"
#include "guiparameter.h"

namespace ogl_examples
{

GUIParameter::GUIParameter()
{
	renderer_names.push_back("RGB Cube");
	renderer_names.push_back("RGB Triangle");
	renderer_names.push_back("Circle SDF");
	renderer_names.push_back("Fourier Series");

	for (int i = 0; i < renderer_names.size(); ++i)
	{
		scene_map.emplace(i, renderer_names[i]);
	}
}

GUIParameter::~GUIParameter() = default;

} // namespace ogl_examples
