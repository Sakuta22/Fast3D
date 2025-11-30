#pragma once
#include <vector>
#include "../Matrix/Matrix.h"
#include "../Polygon/Polygon.h"

struct Object {
	std::vector<Fast3d::Polygon> data;

	Object();
	Object(const std::vector<Fast3d::Polygon> data);

	void Rotation(const Matrix& mat);
};