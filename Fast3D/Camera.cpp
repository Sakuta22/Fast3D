#include "Camera.h"

Camera::Camera() : position(), direction(), viewport() {}
Camera::Camera(const Point position, const Vector direction, const ViewPort viewport) : position(position), direction(direction), viewport(viewport) {}

void Camera::MoveToDiff(const float diffx, const float diffy, const float diffz) {
	this->position += Point(diffx, diffy, diffz);
}