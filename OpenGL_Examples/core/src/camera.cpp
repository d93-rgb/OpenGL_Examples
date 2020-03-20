#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace ogl_examples
{
Camera::Camera(glm::mat4 lookAt) :
	cameraToWorld(std::move(glm::inverse(lookAt))),
	worldToCamera(std::move(lookAt)),
	worldToRaster(1)
{

}

OrthographicCamera::OrthographicCamera(glm::mat4 lookAt, 
	float left, float right, float top, float bottom, float near, float far) :
	Camera(lookAt)
{
	worldToRaster = glm::ortho(left, right, bottom, top, near, far) * worldToCamera;
}

void OrthographicCamera::update()
{

}

PerspectiveCamera::PerspectiveCamera(glm::mat4 lookAt,
	float fovy, float aspect_ratio, float near, float far) :
	Camera(lookAt)
{
	worldToRaster = glm::perspective(fovy, aspect_ratio, near, far) * worldToCamera;

}

void PerspectiveCamera::update()
{

}

} // namespace ogl_examples