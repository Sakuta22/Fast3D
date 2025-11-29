#pragma once
#include <vector>
#include "Vector.h"

namespace Fast3d {

struct Polygon {
	std::vector<Vector> data;

	Polygon();
	Polygon(const std::vector<Vector> data);
};

}