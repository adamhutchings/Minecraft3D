#include "camera.hpp"

#include <application.hpp>
#include <world/blocks.hpp>

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
	if (pitch_ < -89.0f) pitch_ = -89.0f;
	if (pitch_ >  89.0f) pitch_ =  89.0f;
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
	location.x += quantity * cos(glm::radians(degrees + yaw_));
	location.z += quantity * sin(glm::radians(degrees + yaw_));
}

bool Camera::get_block_break_location(int& x, int& y, int &z) {
	
	// We step forward the camera by some increment.
	static const float step = 0.01f;

	// We also stop searching after a given ABSOLUTE distance.
	static const float search_limit = 5.0f;

	// The current position we're checking.
	float cx = location.x, cy = location.y, cz = location.z;

	// How far we've gone so far.
	float distance_searched = 0.0f;

	// The increment we travel by.
	auto increment = glm::vec3(
		 cos(glm::radians(pitch_)) * cos(glm::radians(yaw_)),
		-sin(glm::radians(pitch_)),
		 cos(glm::radians(pitch_)) * sin(glm::radians(yaw_))
	);

	increment = glm::normalize(increment) * step;

	// Also, we want to avoid repeatedly checking the same
	// block in the world, so cache the current position so
	// we can avoid checking. Start them all at a value not
	// in the world so we make sure to check the first iter.
	int
		last_checked_x = -1
	   ,last_checked_y = -1
	   ,last_checked_z = -1
	;

	bool block_found = false;

	for ( ; distance_searched < search_limit; distance_searched += step) {

		if (
			last_checked_x == (int) cx
		&&  last_checked_y == (int) cy
		&&  last_checked_z == (int) cz
		)
			; // Don't bother checking a given block more than once
		else {

			// Set cached value
			last_checked_x = (int) cx,
			last_checked_y = (int) cy,
			last_checked_z = (int) cz;

			BlockType block;

			bool block_exists = global_app->world->get_block_at(
				last_checked_x, last_checked_y, last_checked_z, block
			);

			if (block != BlockType::AIR_BLOCK && block_exists) {
				block_found = true;
				break;
			}

		}

		cx += increment.x;
		cy += increment.y;
		cz += increment.z;

	}

	if (block_found) {
		x = cx;
		y = cy;
		z = cz;
	}

	return block_found;

}

Camera global_camera{};
