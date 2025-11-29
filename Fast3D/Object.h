#pragma once
#include <vector>
#include "Matrix.h"
#include "Polygon.h"

struct Object {
	std::vector<Fast3d::Polygon> data;

	Object();
	Object(const std::vector<Fast3d::Polygon> data);

	void Rotation(const Matrix& mat);
};