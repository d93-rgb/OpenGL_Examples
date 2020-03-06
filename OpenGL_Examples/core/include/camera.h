#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class Camera
{
public:

protected:
	glm::mat4 cameraToWorld;
	glm::mat4 worldToCamera;
};


class OrthographicCamera : public Camera
{
public:

protected:
};


class PerspectiveCamera : public Camera
{
public:

protected:
};

} // namespace ogl_examples