#include "camera.h"
#include "guiparameter.h"

#include "glm/gtc/matrix_transform.hpp"

namespace ogl_examples
{
Camera::Camera(std::shared_ptr<GUIParameter> gui_params, glm::mat4 lookAt) noexcept :
	gui_params(gui_params),
	cameraToWorld(std::move(glm::inverse(lookAt))),
	worldToCamera(std::move(lookAt)),
	worldToRaster(1)
{

}

OrthographicCamera::OrthographicCamera(std::shared_ptr<GUIParameter> gui_params,
	glm::mat4 lookAt,
	float left, float right, float top, float bottom, float near_val, float far_val,
	float* zoom_factor,
	glm::vec2* translation) noexcept :
	Camera(std::move(gui_params), std::move(lookAt)),
	half_width((right - left) * 0.5),
	half_height((top - bottom) * 0.5),
	aspect_ratio(half_width / half_height), inv_aspect_ratio(1.0f / aspect_ratio),
	near_val(near_val), far_val(far_val),
	zoom_factor(zoom_factor),
	translation_vec(translation),
	translation_factors(2.0f / this->gui_params->screen_width,
		(2.0f / aspect_ratio) / this->gui_params->screen_height)
{
	worldToRaster = glm::ortho(left, right, bottom, top, near_val, far_val) * worldToCamera;
}

OrthographicCamera::OrthographicCamera(std::shared_ptr<GUIParameter> gui_params,
	glm::mat4 lookAt,
	float width, float aspect_ratio, float near_val, float far_val,
	float* zoom_factor,
	glm::vec2* translation) noexcept :
	Camera(std::move(gui_params), std::move(lookAt)),
	half_width(width * 0.5),
	half_height(half_width / aspect_ratio),
	aspect_ratio(aspect_ratio), inv_aspect_ratio(1.0f / aspect_ratio),
	near_val(near_val), far_val(far_val),
	zoom_factor(zoom_factor),
	translation_vec(translation),
	translation_factors(2.0f / this->gui_params->screen_width,
		(2.0f) / this->gui_params->screen_height)
{
	worldToRaster = glm::ortho(
		-half_width,
		half_width,
		-half_height,
		half_height,
		near_val,
		far_val) * worldToCamera;
}

void OrthographicCamera::update() noexcept
{
	static glm::vec2 t_vec_transformed;
	static glm::vec2 current_translation_factors;
	static float half_width_tmp;
	static float half_height_tmp;

	if (!zoom_factor || !translation_vec)
	{
		return;
	}

	half_width_tmp = half_width * (*zoom_factor);
	half_height_tmp = half_height * (*zoom_factor);

	current_translation_factors.x = translation_factors.x * half_width_tmp;
	current_translation_factors.y = translation_factors.y * half_height_tmp;

	t_vec_transformed = current_translation_factors * (*translation_vec);

	worldToRaster = glm::ortho(
		-half_width_tmp + (t_vec_transformed).x,
		half_width_tmp + (t_vec_transformed).x,
		-half_height_tmp + (t_vec_transformed).y,
		half_height_tmp + (t_vec_transformed).y,
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

void PerspectiveCamera::update() noexcept
{

}

} // namespace ogl_examples