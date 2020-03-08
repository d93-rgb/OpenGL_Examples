#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class Camera
{
public:
	Camera::Camera(glm::mat4 lookAt);

protected:
	glm::mat4 cameraToWorld;
	glm::mat4 worldToCamera;
	glm::mat4 worldToRaster;
};


class OrthographicCamera : public Camera
{
public:
	OrthographicCamera::OrthographicCamera(glm::mat4 lookAt,
		float left, float right, float top, float bottom, float near, float far);

protected:
};


class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera::PerspectiveCamera(glm::mat4 lookAt,
		float fovy, float aspect_ratio, float near, float far);


protected:
};

} // namespace ogl_examples