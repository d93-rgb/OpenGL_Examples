#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace ogl_examples
{
Camera::Camera(std::shared_ptr<GUIParameter> gui_params, glm::mat4 lookAt) noexcept :
	cameraToWorld(std::move(glm::inverse(lookAt))),
	worldToCamera(std::move(lookAt)),
	worldToRaster(1)
{

}

OrthographicCamera::OrthographicCamera(std::shared_ptr<GUIParameter> gui_params, 
	glm::mat4 lookAt,
	float left, float right, float top, float bottom, float near_val, float far_val) noexcept :
	Camera(std::move(gui_params), std::move(lookAt)),
	half_width((right - left) * 0.5),
	half_height((top - bottom) * 0.5),
	aspect_ratio(half_width / half_height), inv_aspect_ratio(1.0f / aspect_ratio),
	near_val(near_val), far_val(far_val)
{
	worldToRaster = glm::ortho(left, right, bottom, top, near_val, far_val) * worldToCamera;
}

OrthographicCamera::OrthographicCamera(std::shared_ptr<GUIParameter> gui_params,
	glm::mat4 lookAt,
	float width, float aspect_ratio, float near_val, float far_val) noexcept :
	Camera(std::move(gui_params), std::move(lookAt)), 
	half_width(width * 0.5),
	half_height(half_width / aspect_ratio),
	aspect_ratio(aspect_ratio), inv_aspect_ratio(1.0f / aspect_ratio),
	near_val(near_val), far_val(far_val)
{
	worldToRaster = glm::ortho(
		-half_width, 
		half_width, 
		-half_height,
		half_height,
		near_val,
		far_val) * worldToCamera;
}

void OrthographicCamera::update(float zoom_factor) noexcept
{
	float half_width_tmp = half_width * zoom_factor;
	float half_height_tmp = half_height * zoom_factor;

	worldToRaster = glm::ortho(
		-half_width_tmp,
		half_width_tmp,
		-half_height_tmp,
		half_height_tmp,
		near_val,
		far_val) * worldToCamera;
}

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<GUIParameter> gui_params,
	glm::mat4 lookAt,
	float fovy, float aspect_ratio, float near_val, float far_val) noexcept :
	Camera(std::move(gui_params), lookAt)
{
	worldToRaster = glm::perspective(fovy, aspect_ratio, near_val, far_val) * worldToCamera;

}

void PerspectiveCamera::update(float zoom_factor) noexcept
{

}

} // namespace ogl_examples