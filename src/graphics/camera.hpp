/**
 * Camera - adjust the viewing angle which
 * in-game objects are rendered from.
 */

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

enum ViewMode {
	SET,
	ADJUST,
};

class Camera {

private:
	float pitch_, yaw_, fov, nearclip, farclip;
	glm::vec3 location;

public:

	Camera();
	// Change the pitch
	void pitch(float angle, ViewMode mode);
	// Change the yaw
	void yaw  (float angle, ViewMode mode);
	void position(float x, float y, float z, ViewMode mode);
	// Multiply this in the fragment shader to make a view.
	glm::mat4 get_transform_matrix();
	// Move in a given direction degrees from straight ahead.
	void move(float quantity, float degrees);
	// Find the location of the first solid block to break.
	// Returns false if no block was found.
	bool get_block_break_location(int& x, int& y, int& z);

	inline int x() { return location.x; }
	inline int y() { return location.y; }
	inline int z() { return location.z; }

};
