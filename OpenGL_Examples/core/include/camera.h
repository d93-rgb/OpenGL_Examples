#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class Camera
{
public:

protected:
	glm::mat4 lookAt;
	glm::mat4 cameraToWorld;
	glm::mat4 worldToCamera;
};


class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(glm::mat4 lookAt);

protected:
};


class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::mat4 lookAt);

protected:
};

} // namespace ogl_examples