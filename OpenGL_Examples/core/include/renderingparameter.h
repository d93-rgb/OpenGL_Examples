#pragma once
#include "opengl_examples.h"
#include "GL/gl3w.h"

namespace ogl_examples
{

class RenderingParameter
{
public:
	const WindowManager* wm;

	RenderingParameter(const WindowManager* wm);
};


class TriangleRendererParameter : public RenderingParameter
{
public:
	TriangleRendererParameter(const WindowManager* wm);

};

class CubeRendererParameter : public RenderingParameter
{
public:
	CubeRendererParameter(const WindowManager* wm);

};

} // namespace ogl_examples