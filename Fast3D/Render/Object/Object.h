#pragma once
#include <vector>
#include "../Matrix/Matrix.h"
#include "../Polygon/Polygon.h"

namespace Console3D {

struct Object {
	std::vector<Console3D::Polygon> data;

	Object();
	Object(const std::vector<Console3D::Polygon> data);

	void Rotation(const Matrix& mat);
};

}