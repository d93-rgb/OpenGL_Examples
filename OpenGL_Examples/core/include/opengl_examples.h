#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <glog/logging.h>

namespace ogl_examples
{

class WindowManager;
class ShaderCompiler;
class Renderer;

class RenderingParameter;
class TriangleRendererParameter;

extern int current_scene_flag;
extern int old_scene_flag;


} // namespace ogl_examples