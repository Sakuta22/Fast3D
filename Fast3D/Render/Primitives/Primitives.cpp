#include "Primitives.h"
using namespace Console3D;

std::vector<Console3D::Polygon> Console3D::generateSphere(float radius, int rings, int sectors) {
	std::vector<Console3D::Polygon> spherePolys;
	const float PI = 3.14159265359f;

	for (int i = 0; i < rings; ++i) {
		float lat0 = PI * (-0.5f + (float)(i) / rings);
		float lat1 = PI * (-0.5f + (float)(i + 1) / rings);

		float z0 = radius * sin(lat0);
		float zr0 = radius * cos(lat0);

		float z1 = radius * sin(lat1);
		float zr1 = radius * cos(lat1);

		for (int j = 0; j < sectors; ++j) {
			float lng0 = 2 * PI * (float)(j) / sectors;
			float lng1 = 2 * PI * (float)(j + 1) / sectors;

			float x0 = zr0 * cos(lng0);
			float y0 = zr0 * sin(lng0);

			float x1 = zr0 * cos(lng1);
			float y1 = zr0 * sin(lng1);

			float x2 = zr1 * cos(lng1);
			float y2 = zr1 * sin(lng1);

			float x3 = zr1 * cos(lng0);
			float y3 = zr1 * sin(lng0);

			spherePolys.push_back(Console3D::Polygon(std::vector<Vector>{
				Point(x3, y3, z1),
				Point(x2, y2, z1),
				Point(x1, y1, z0),
				Point(x0, y0, z0),
			}));
		}
	}
	return spherePolys;
}

std::vector<Console3D::Polygon> Console3D::generateTorus(float radius, float tubeRadius, int segments, int sides) {
	std::vector<Console3D::Polygon> torusPolys;
	const float PI = 3.14159265359f;

	for (int i = 0; i < segments; ++i) {
		for (int j = 0; j < sides; ++j) {
			float u = (float)i / segments * 2 * PI;
			float u_next = (float)(i + 1) / segments * 2 * PI;

			float v = (float)j / sides * 2 * PI;
			float v_next = (float)(j + 1) / sides * 2 * PI;

			auto getPoint = [&](float angle_u, float angle_v) -> Point {
				float x = (radius + tubeRadius * cos(angle_v)) * cos(angle_u);
				float y = (radius + tubeRadius * cos(angle_v)) * sin(angle_u);
				float z = tubeRadius * sin(angle_v);
				return Point(x, y, z);
				};

			Point p0 = getPoint(u, v);
			Point p1 = getPoint(u_next, v);
			Point p2 = getPoint(u_next, v_next);
			Point p3 = getPoint(u, v_next);

			torusPolys.push_back(Console3D::Polygon(std::vector<Vector>{
				p0,
				p1,
				p2,
				p3,
			}));
		}
	}
	return torusPolys;
}
