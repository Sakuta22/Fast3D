#pragma once
#include "../Point/Point.h"
#include "../Vector/Vector.h"
#include "../ViewPort/ViewPort.h"

namespace Console3D {

struct Camera {
	Point position;
	Vector direction;
	ViewPort viewport;

	Camera();
	Camera(const Point position, const Vector direction, const ViewPort viewport);

	void MoveToDiff(const float diffx, const float diffy, const float diffz);
};

}