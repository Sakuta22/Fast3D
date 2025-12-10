#pragma once
#include <vector>
#include "../Polygon/Polygon.h"

namespace Console3D {

	std::vector<Console3D::Polygon> generateSphere(float radius, int rings, int sectors);

	std::vector<Console3D::Polygon> generateTorus(float radius, float tubeRadius, int segments, int sides);

}