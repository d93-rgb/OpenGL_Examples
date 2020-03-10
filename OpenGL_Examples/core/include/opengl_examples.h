#pragma once

#define GLM_FORCE_RADIANS
#include <memory>
#include <string>
#include <vector>

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

class Renderer;
class CubeRenderer;

class RenderingManager;

class EventHandler;
class TriangleRendererEventHandler;
class CubeRendererEventHandler;

class Camera;

class GUIParameter;

extern int current_scene_flag;
extern int old_scene_flag;

} // namespace ogl_examples