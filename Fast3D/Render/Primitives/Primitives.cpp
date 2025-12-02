#include "Primitives.h"
//using namespace Console3D;

std::vector<Console3D::Polygon> Console3D::generateSphere(float radius, int rings, int sectors) {
	std::vector<Console3D::Polygon> spherePolys;
	const float PI = 3.14159265359f;

	for (int i = 0; i < rings; ++i) {
		// Вычисляем углы широты (от верхнего полюса вниз)
		float lat0 = PI * (-0.5f + (float)(i) / rings);
		float lat1 = PI * (-0.5f + (float)(i + 1) / rings);

		// Координаты Y и радиусы колец на данной высоте
		float z0 = radius * sin(lat0);
		float zr0 = radius * cos(lat0);

		float z1 = radius * sin(lat1);
		float zr1 = radius * cos(lat1);

		for (int j = 0; j < sectors; ++j) {
			// Вычисляем углы долготы (вокруг оси Y)
			float lng0 = 2 * PI * (float)(j) / sectors;
			float lng1 = 2 * PI * (float)(j + 1) / sectors;

			// Вычисляем X и Z для 4 точек квадрата (полигона)
			float x0 = zr0 * cos(lng0);
			float y0 = zr0 * sin(lng0);

			float x1 = zr0 * cos(lng1);
			float y1 = zr0 * sin(lng1);

			float x2 = zr1 * cos(lng1);
			float y2 = zr1 * sin(lng1);

			float x3 = zr1 * cos(lng0);
			float y3 = zr1 * sin(lng0);

			// Создаем полигон (порядок точек важен для нормалей, здесь CCW)
			// Формируем грань из 4-х точек
			spherePolys.push_back(Console3D::Polygon(std::vector<Vector>{
				Point(x3, y3, z1),  // Верх-Лево
					Point(x2, y2, z1), // Верх-Право
					Point(x1, y1, z0), // Низ-Право
					Point(x0, y0, z0), // Низ-Лево
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
			// Углы для текущего сегмента (u) и следующего (u_next)
			float u = (float)i / segments * 2 * PI;
			float u_next = (float)(i + 1) / segments * 2 * PI;

			// Углы для сечения трубки (v) и следующего (v_next)
			float v = (float)j / sides * 2 * PI;
			float v_next = (float)(j + 1) / sides * 2 * PI;

			// Лямбда-функция для расчета точки на торе
			auto getPoint = [&](float angle_u, float angle_v) -> Point {
				float x = (radius + tubeRadius * cos(angle_v)) * cos(angle_u);
				float y = (radius + tubeRadius * cos(angle_v)) * sin(angle_u); // Лежит на плоскости XY
				float z = tubeRadius * sin(angle_v);
				return Point(x, y, z);
				};

			// Генерируем 4 точки квадрата
			Point p0 = getPoint(u, v);
			Point p1 = getPoint(u_next, v);
			Point p2 = getPoint(u_next, v_next);
			Point p3 = getPoint(u, v_next);

			// Добавляем полигон
			// Порядок вершин: CW (по часовой стрелке), чтобы соответствовать вашему кубу
			torusPolys.push_back(Console3D::Polygon(std::vector<Vector>{
				p3, // Верх-Лево
					p0, // Низ-Лево
					p1,  // Низ-Право
					p2, // Верх-Право
			}));
		}
	}
	return torusPolys;
}
