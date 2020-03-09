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

class EventHandler;
class TriangleRendererEventHandler;
class CubeRendererEventHandler;

extern int current_scene_flag;
extern int old_scene_flag;

extern bool OGL_EXAMPLES_UPDATE_CUBE_VERTICES;
extern glm::mat4 cube_rot_mat;

} // namespace ogl_examples