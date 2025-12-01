#pragma once
#include <vector>
#include "../Vector/Vector.h"

namespace Console3D {

struct Polygon {
	std::vector<Vector> data;

	Polygon();
	Polygon(const std::vector<Vector> data);
};

}