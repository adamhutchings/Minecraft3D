#include "camera.hpp"

Camera::Camera() {

	// Lots of zeros!
	position(0, 0, 0, SET);
	yaw     (0, SET);
	pitch   (0, SET);

	fov      = 70.0f;

	nearclip = 0.01f;
	farclip  = 100.0f;

}

void Camera::pitch(float angle, ViewMode mode) {
	if (mode == ADJUST) {
		pitch_ += angle;
	} else if (mode == SET) {
		pitch_  = angle;
	}
}

void Camera::yaw(float angle, ViewMode mode) {
	if (mode == ADJUST) {
		yaw_ += angle;
	} else if (mode == SET) {
		yaw_  = angle;
	}
}

void Camera::position(float x, float y, float z, ViewMode mode) {
	if (mode == ADJUST) {
		location.x += x;
		location.y += y;
		location.z += z;
	} else if (mode == SET) {
		location.x  = x;
		location.y  = y;
		location.z  = z;
	}
}

glm::mat4 Camera::get_transform_matrix() {

	auto proj = glm::perspective(glm::radians(fov), 1.0f, nearclip, farclip);

    glm::vec3 direction;
    direction.x =  cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
    direction.y = -sin(glm::radians(pitch_));
    direction.z =  cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));
    auto view   = glm::lookAt(location, location + glm::normalize(direction), glm::vec3(0, 1, 0));

    return proj * view;

}

void Camera::move(float quantity, float degrees) {
	location.x += quantity * cos(glm::radians(degrees + yaw));
	location.z += quantity * sin(glm::radians(degrees + yaw));
}

Camera global_camera{};
