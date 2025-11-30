#include <iostream>
#include "Render/Render.h"
#include <Windows.h>
#include <vector>
using namespace std;

vector<Fast3d::Polygon> generateSphere(float radius, int rings, int sectors) {
	vector<Fast3d::Polygon> spherePolys;
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
			spherePolys.push_back(Fast3d::Polygon(vector<Vector>{
				Point(x3, y3, z1),  // Верх-Лево
					Point(x2, y2, z1), // Верх-Право
					Point(x1, y1, z0), // Низ-Право
					Point(x0, y0, z0), // Низ-Лево
			}));
		}
	}
	return spherePolys;
}

std::vector<Fast3d::Polygon> generateTorus(float radius, float tubeRadius, int segments, int sides) {
	std::vector<Fast3d::Polygon> torusPolys;
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
			torusPolys.push_back(Fast3d::Polygon(std::vector<Vector>{
				p3, // Верх-Лево
					p0, // Низ-Лево
					p1,  // Низ-Право
					p2, // Верх-Право
			}));
		}
	}
	return torusPolys;
}

int main() {
	vector<Fast3d::Polygon> cube = {
		Fast3d::Polygon(vector<Vector> {Point(-0.5f, -0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f), Point(0.5f,  0.5f, -0.5f), Point(0.5f, -0.5f, -0.5f)}), //front
		Fast3d::Polygon(vector<Vector> {Point(0.5f,  0.5f, -0.5f), Point(0.5f,  0.5f,  0.5f), Point(0.5f, -0.5f,  0.5f), Point(0.5f, -0.5f, -0.5f)}), //right
		Fast3d::Polygon(vector<Vector> {Point(-0.5f, -0.5f,  0.5f), Point(0.5f, -0.5f,  0.5f), Point(0.5f,  0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //back
		Fast3d::Polygon(vector<Vector> {Point(-0.5f,  0.5f, -0.5f), Point(-0.5f, -0.5f, -0.5f), Point(-0.5f, -0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //left
		Fast3d::Polygon(vector<Vector> {Point(-0.5f,  0.5f,  0.5f), Point(0.5f,  0.5f,  0.5f), Point(0.5f,  0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f)}), //top
		Fast3d::Polygon(vector<Vector> {Point(-0.5f, -0.5f, -0.5f), Point(0.5f, -0.5f, -0.5f), Point(0.5f, -0.5f,  0.5f), Point(-0.5f, -0.5f,  0.5f)}), //bottom
	};
	Object Cube(cube);

	std::vector<Fast3d::Polygon> torus = generateTorus(1.5f, 0.33f, 30, 30);
	vector<Fast3d::Polygon> sphereData = generateSphere(0.5f, 12, 12);
	vector<Fast3d::Polygon> sphereData1 = generateSphere(0.50f, 6, 6);
	Scene MScene({ torus, sphereData, cube });
	//Scene MScene({Cube});

	Camera MCamera(Point(0.f, 0.f, -6.f), Vector(0.f, 0.f, 1.f), ViewPort());

	//Sleep(4000);
	Matrix mat, matx, maty, matz;
	Vector axis(Point(300.f, 1000.f, 650.f));
	axis.Normalize();
	Matrix::CreateRotateMatrix(mat, axis, 3.f);
	Matrix::CreateRotateMatrix(matx, Vector(0.f, 1.f, 0.f), 4.f);
	Matrix::CreateRotateMatrix(maty, Vector(1.f, 0.f, 0.f), 2.f);
	Matrix::CreateRotateMatrix(matz, Vector(1.f, 0.33f, 1.f).Normalized(), 2.f);
	Screen MScreen;
	Render render(MScene, MCamera);
	//render.settings.RenderInvisiblePolygons = true;
	Render::Settings::RenderInvisiblePolygons = false;
	render.camera.MoveToDiff(0.0f, 0.f, 2.5f);
	int q = 0, qq = 0, w = 0;
	while (true) {
		w++;
		if (qq == 1)
			q--;
		if (qq == 0)
			q++;
		if (q == 20)
			qq = 1;
		if (q == -20)
			qq = 0;

		//if (w % 20 == 0)
		MScreen.SetScreenNow();

		render.scene.data[0].Rotation(mat);
		//render.scene.scene[0].Rotation(matz);
		render.scene.data[1].Rotation(matx);
		render.scene.data[1].Rotation(matz);
		//render.scene.scene[1].Rotation(mat);
		//render.scene.scene[2].Rotation(matx);
		render.scene.data[2].Rotation(maty);
		render.scene.data[2].Rotation(matz);
		//render.camera.MoveToDiff((qq == 0 ? -0.05f : 0.05f), (qq == 1 ? -0.02f : 0.02f), (qq == 1 ? -0.05f : 0.05f));

		render.Start(MScreen.Buffers[1 - MScreen.ActiveBuffer]);
		MScreen.SwapBuffers();

		Sleep(17);
	}
}