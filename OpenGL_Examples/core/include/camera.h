#pragma once
#include "opengl_examples.h"

namespace ogl_examples
{

class Camera
{
public:
	Camera(std::shared_ptr<GUIParameter> gui_params, glm::mat4 lookAt) noexcept;

	glm::mat4 cameraToWorld;
	glm::mat4 worldToCamera;
	glm::mat4 worldToRaster;

	virtual void update(float zoom_factor) noexcept = 0;
protected:
	std::shared_ptr<GUIParameter> gui_params;
};


class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(std::shared_ptr<GUIParameter> gui_params, 
		glm::mat4 lookAt,
		float left, float right, float top, float bottom, float near_val, float far_val) noexcept;

	OrthographicCamera(std::shared_ptr<GUIParameter> gui_params,
		glm::mat4 lookAt,
		float width, float aspect_ratio, float near_val, float far_val) noexcept;

	void update(float zoom_factor) noexcept override;
protected:
	float half_width;
	float half_height;
	float aspect_ratio;
	float inv_aspect_ratio;
	float near_val;
	float far_val;
};


class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(std::shared_ptr<GUIParameter> gui_params,
		glm::mat4 lookAt,
		float fovy, float aspect_ratio, float near_val, float far_val) noexcept;

	void update(float zoom_factor) noexcept override;
protected:
};

} // namespace ogl_examples