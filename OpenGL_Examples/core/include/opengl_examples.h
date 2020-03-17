#pragma once

#define GLM_FORCE_RADIANS
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>

#include <glog/logging.h>

namespace ogl_examples
{

class WindowManager;
class ShaderCompiler;
class Shader;
class Renderer;
class GraphicsContext;
class GUIParameter;
class Camera;

class RenderingManager;

class Renderer;
class TriangleRenderer;
class CubeRenderer;
class FourierSeriesRenderer;

class RenderingParameter;
class TriangleRendererParameter;
class CubeRendererParameter;
class FourierSeriesRendererParameter;

class EventHandler;
class TriangleRendererEventHandler;
class CubeRendererEventHandler;
class FourierSeriesRendererEventHandler;

constexpr double TWO_PI = 2 * M_PI;
} // namespace ogl_examples