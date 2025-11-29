#pragma once
#include <vector>
#include "Point.h"

namespace Fast3d {

struct Polygon {
	std::vector<Point> data;

	Polygon();
	Polygon(std::vector<Point> data);
};

}