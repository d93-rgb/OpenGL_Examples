#pragma once

#define GLM_FORCE_RADIANS
#include <memory>
#include <glm/glm.hpp>

#include <glog/logging.h>

namespace ogl_examples
{

class WindowManager;
class ShaderCompiler;
class Renderer;
class GraphicsContext;

class RenderingParameter;
class TriangleRendererParameter;
class CubeRendererParameter;

extern int current_scene_flag;
extern int old_scene_flag;


} // namespace ogl_examples