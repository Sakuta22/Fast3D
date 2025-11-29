#pragma once
#include "Point.h"
#include "Vector.h"
#include "ViewPort.h"

struct Camera {
	Point position;
	Vector direction;
	ViewPort viewport;

	Camera();
	Camera(const Point position, const Vector direction, const ViewPort viewport);

	void MoveToDiff(const float diffx, const float diffy, const float diffz);
};