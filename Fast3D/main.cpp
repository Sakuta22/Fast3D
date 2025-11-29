#include <iostream>
#include "Point.h"
#include "Vector.h"
#include "Matrix.h"
#include "Polygon.h"
#include "Object.h"
#include "ViewPort.h"
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <vector>
#include <map>
namespace Fast3d {} ;

using namespace std;

HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

struct Screen;
struct Camera;
struct Scene;
struct Render;

struct Camera {
	Point position;
	Vector direction;
	ViewPort viewport;

	Camera() : position(), direction(), viewport() {}
	Camera(Point position, Vector direction, ViewPort viewport) : position(position), direction(direction), viewport(viewport) {}

	void MoveToDiff(float DiffX, float DiffY, float DiffZ) {
		this->position.x += DiffX;
		this->position.y += DiffY;
		this->position.z += DiffZ;
	}

	Object WorldToViewPort(Object WorldObject) {
		Object ViewPortObject;
		for (Fast3d::Polygon WorldPolygon : WorldObject.data) {
			Fast3d::Polygon ViewPortPolygon;
			for (Vector WorldVector : WorldPolygon.data) {
				Vector ViewPortVector;

				ViewPortVector.direction.x = (WorldVector.direction.x - this->position.x) * this->viewport.deep / (WorldVector.direction.z - this->position.z);
				ViewPortVector.direction.y = (WorldVector.direction.y - this->position.y) * this->viewport.deep / (WorldVector.direction.z - this->position.z);
				ViewPortVector.direction.z = this->viewport.deep;

				ViewPortPolygon.data.push_back(ViewPortVector);
			}
			ViewPortObject.data.push_back(ViewPortPolygon);
		}
		return ViewPortObject;
	}
};

struct Screen {
	static int width, height;
	static float AspectRatio, PixelRatio;
	static wchar_t* screen;
	HANDLE Buffers[2];
	int ActiveBuffer;

	Screen() : ActiveBuffer(0) {
		this->Buffers[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		this->Buffers[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(this->Buffers[0], &cursorInfo);
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(this->Buffers[0], &cursorInfo);
		SetConsoleCursorInfo(this->Buffers[1], &cursorInfo);

		SetScreenNow();
		screen = new wchar_t[this->width * this->height];
		FreeScreen();
		SetAspect();
	}

	bool isBufferChange() {
		CONSOLE_SCREEN_BUFFER_INFO Info;
		GetConsoleScreenBufferInfo(StdOut, &Info);

		int Iwidth = Info.srWindow.Right - Info.srWindow.Left + 1;
		int Iheight = Info.srWindow.Bottom - Info.srWindow.Top + 1;

		if (Iwidth != this->width && Iheight != this->height)
			return true;
		return false;
	}

	void FreeScreen() {
		if (screen) {
			wmemset(screen, L' ', width * height);
		}
	}

	void SetAspect() {
		AspectRatio = (float)this->height / (float)this->width;
		PixelRatio = 24.f / 11.f;
	}

	void SetScreenNow() {
		CONSOLE_SCREEN_BUFFER_INFO Info;
		GetConsoleScreenBufferInfo(this->Buffers[this->ActiveBuffer], &Info);

		int newW = Info.srWindow.Right - Info.srWindow.Left + 1;
		int newH = Info.srWindow.Bottom - Info.srWindow.Top + 1;

		if (newW != this->width || newH != this->height) {
			this->width = newW;
			this->height = newH;

			if (screen) delete[] screen;
			screen = new wchar_t[this->width * this->height];

			this->SetAspect();

			/*CloseHandle(this->Buffers[0]);
			CloseHandle(this->Buffers[1]);
			this->Buffers[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
			this->Buffers[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);*/

			COORD coord = { this->width, this->height };
			SetConsoleScreenBufferSize(this->Buffers[0], coord);
			SetConsoleScreenBufferSize(this->Buffers[1], coord);

			SMALL_RECT rect;
			rect.Left = 0;
			rect.Top = 0;
			rect.Right = this->width - 1;
			rect.Bottom = this->height - 1;
			SetConsoleWindowInfo(this->Buffers[0], TRUE, &rect);
			SetConsoleWindowInfo(this->Buffers[1], TRUE, &rect);
		}

		FreeScreen();
	}

	void SetBuffer(HANDLE Handle)
	{
		_COORD coord = { this->width, this->height };
		SetConsoleScreenBufferSize(Handle, coord);

		_SMALL_RECT Rect = { 0, 0, this->width - 1, this->height - 1 };
		SetConsoleWindowInfo(Handle, TRUE, &Rect);
	}

	void SwapBuffers() {
		SetConsoleActiveScreenBuffer(this->Buffers[1 - this->ActiveBuffer]);
		this->ActiveBuffer = 1 - this->ActiveBuffer;
		FreeScreen();
	}
};
int Screen::width = 0;
int Screen::height = 0;
float Screen::AspectRatio = 0.0f;
float Screen::PixelRatio = 0.0f;
wchar_t* Screen::screen = nullptr;

struct Scene {
	vector<Object> scene;

	Scene() : scene() {}
	Scene(vector<Object> scene) : scene(scene) {}
};

struct Render {
	struct Settings {
		bool RenderInvisiblePolygons = false;
	};

	Settings settings;
	Scene scene;
	Camera camera;

	Render(Scene scene, Camera camera) : scene(scene), camera(camera) {}

	void Start(HANDLE& buffer) {
		for (Object WorldObject : scene.scene) {
			this->PrintPolygon(WorldObject, this->camera);
			this->FillBuffer(buffer);
		}
	}

	Vector GetNormale(Fast3d::Polygon& WorldPolygon) {
		Vector vec1(WorldPolygon.data[1] - WorldPolygon.data[0]);
		Vector vec2(WorldPolygon.data[2] - WorldPolygon.data[0]);
		Vector Normal = Vector::Cross(vec2, vec1);

		if (Normal.Length() < 0.00001f) {
			Vector vec3(WorldPolygon.data[2] - WorldPolygon.data[0]);
			Vector vec4(WorldPolygon.data[3] - WorldPolygon.data[0]);
			Normal = Vector::Cross(vec4, vec3);
		}

		return Normal.Normalized();
	}

	void PrintPolygon(Object WorldObject, Camera Cam) {
		for (Fast3d::Polygon WorldPolygon : WorldObject.data) {
			if (!this->settings.RenderInvisiblePolygons) {
				Vector Normal = this->GetNormale(WorldPolygon);
				Vector ViewCam(Vector(this->camera.position) - WorldPolygon.data[0]);

				float DotProduct = Vector::Dot(Normal, ViewCam);
				if (DotProduct > 0.f)
					continue;
			}

			Fast3d::Polygon BufferPolygon;
			for (Vector WorldVector : WorldPolygon.data) {
				//WORLDVECTOR TO VIEWPORTVECTOR
				Vector ViewPortVector;
				ViewPortVector.direction.x = (WorldVector.direction.x - Cam.position.x) * Cam.viewport.deep / (WorldVector.direction.z - Cam.position.z);
				ViewPortVector.direction.y = (WorldVector.direction.y - Cam.position.y) * Cam.viewport.deep / (WorldVector.direction.z - Cam.position.z);
				ViewPortVector.direction.z = Cam.viewport.deep;

				//VIEWPORTVECTOR TO BUFFERPOINT
				ViewPortVector.direction.x *= Screen::AspectRatio * Screen::PixelRatio;
				Point BufferPoint;
				BufferPoint.x = Screen::width * (ViewPortVector.direction.x + Cam.viewport.width / 2.f);
				BufferPoint.y = Screen::height * (-ViewPortVector.direction.y + Cam.viewport.height / 2.f);

				//PUSH TO BUFFERNPOINT TO BUFFERPOLYGON
				BufferPolygon.data.push_back(BufferPoint);
			}

			int BPS = BufferPolygon.data.size();
			for (int i = 0; i < BPS; i++) {
				Point ScreenPoint1 = BufferPolygon.data[i].direction;
				Point ScreenPoint2 = BufferPolygon.data[(i + 1) % BPS].direction;

				PrintLine(ScreenPoint1, ScreenPoint2);
			}
		}
	}

	void PrintLine(Point p1, Point p2) {
		if (p1.y > p2.y)
			swap(p1, p2);

		if (abs(p2.x - p1.x) > abs(p2.y - p1.y)) {
			float y = p1.y, dy = abs((p2.y - p1.y) / (p2.x - p1.x));
			p1.x = round(p1.x), p2.x = round(p2.x);
			if (p1.x <= p2.x) {
				for (int x = p1.x; x <= p2.x; x++, y += dy) {
					if ((0 <= x && x < Screen::width) && (0 <= round(y) && round(y) < Screen::height))
						Screen::screen[int(Screen::width * round(y) + x)] = '*';
				}
			}
			else {
				for (int x = p1.x; x >= p2.x; x--, y += dy) {
					if ((0 <= x && x < Screen::width) && (0 <= round(y) && round(y) < Screen::height))
						Screen::screen[int(Screen::width * round(y) + x)] = '*';
				}
			}
		}
		else {
			float x = p1.x, dx = (p2.y - p1.y == 0.f ? 0.f : (p2.x - p1.x) / (p2.y - p1.y));
			p1.y = round(p1.y), p2.y = round(p2.y);
			for (int y = p1.y; y <= p2.y; y++, x += dx) {
				if ((0 <= round(x) && round(x) < Screen::width) && (0 <= y && y < Screen::height))
					Screen::screen[int(Screen::width * y + round(x))] = '*';
			}
		}
	}

	void FillBuffer(HANDLE& buffer) {
		DWORD useless = NULL;
		WriteConsoleOutputCharacterW(buffer, Screen::screen, Screen::width * Screen::height, { 0, 0 }, &useless);
	}
};

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

		render.scene.scene[0].Rotation(mat);
		//render.scene.scene[0].Rotation(matz);
		render.scene.scene[1].Rotation(matx);
		render.scene.scene[1].Rotation(matz);
		//render.scene.scene[1].Rotation(mat);
		//render.scene.scene[2].Rotation(matx);
		render.scene.scene[2].Rotation(maty);
		render.scene.scene[2].Rotation(matz);
		//render.camera.MoveToDiff((qq == 0 ? -0.05f : 0.05f), (qq == 1 ? -0.02f : 0.02f), (qq == 1 ? -0.05f : 0.05f));

		render.Start(MScreen.Buffers[1 - MScreen.ActiveBuffer]);
		MScreen.SwapBuffers();

		Sleep(17);
	}
}