#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <vector>
#include <map>
using namespace std;

const float M_PI = 3.14159265358979323846;
HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

struct Point;
struct Vector;
struct Matrix;
struct Polygonn;
struct Object;
struct Screen;
struct ViewPort;
struct Camera;
struct Scene;
struct Render;

void multiplie(Point&, Matrix);

struct Point {
	float x, y, z;

	Point() : x(0), y(0), z(0) {}
	Point(float same) : x(same), y(same), z(same) {}
	Point(float x, float y, float z) : x(x), y(y), z(z) {}

	Point operator- (Point &other) const {
		return Point(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	bool operator== (Point& other) const {
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}
};

struct Vector {
	Point xyz;

	Vector() : xyz(0, 0, 1) {}
	Vector(float same) : xyz(same) {}
	Vector(float x, float y, float z) : xyz(x, y, z) {}
	Vector(Point direction) : xyz(direction) {}

	float Length() {
		return sqrt(this->xyz.x * this->xyz.x + this->xyz.y * this->xyz.y + this->xyz.z * this->xyz.z);
	}
	Vector Normalize() {
		float length = this->Length();
		return Vector(this->xyz.x / length, this->xyz.y / length, this->xyz.z / length);
	}
	void Normalize(Vector &vec) {
		float length = this->Length();
		vec.xyz.x = this->xyz.x / length;
		vec.xyz.y = this->xyz.y / length;
		vec.xyz.z = this->xyz.z / length;
	}

	static float GetCos(Vector vec1, Vector vec2) {
		return Vector::Dot(vec1, vec2) / (vec1.Length() * vec2.Length());
	}

	static float Dot(Vector vec1, Vector vec2) {
		return vec1.xyz.x * vec2.xyz.x + vec1.xyz.y * vec2.xyz.y + vec1.xyz.z * vec2.xyz.z;
	}

	static Vector Cross(Vector vec1, Vector vec2) {
		return Vector(
			vec1.xyz.y * vec2.xyz.z - vec1.xyz.z * vec2.xyz.y,
			vec1.xyz.z * vec2.xyz.x - vec1.xyz.x * vec2.xyz.z,
			vec1.xyz.x * vec2.xyz.y - vec1.xyz.y * vec2.xyz.x
		);
	}
};

struct Matrix {
	float matrix[3][3];

	Matrix() : matrix{ {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f} } {}

	static void CreateRotateMatrix(Matrix &Mat, Vector Axis, float Angle) {
		Angle *= M_PI / 180.f;
		
		Mat.matrix[0][0] = cos(Angle) + (1 - cos(Angle)) * pow(Axis.xyz.x, 2.f);
		Mat.matrix[0][1] = (1 - cos(Angle)) * Axis.xyz.x * Axis.xyz.y - sin(Angle) * Axis.xyz.z;
		Mat.matrix[0][2] = (1 - cos(Angle)) * Axis.xyz.x * Axis.xyz.z + sin(Angle) * Axis.xyz.y;
		
		Mat.matrix[1][0] = (1 - cos(Angle)) * Axis.xyz.y * Axis.xyz.x + sin(Angle) * Axis.xyz.z;
		Mat.matrix[1][1] = cos(Angle) + (1 - cos(Angle)) * pow(Axis.xyz.y, 2.f);
		Mat.matrix[1][2] = (1 - cos(Angle)) * Axis.xyz.y * Axis.xyz.z - sin(Angle) * Axis.xyz.x;

		Mat.matrix[2][0] = (1 - cos(Angle)) * Axis.xyz.z * Axis.xyz.x - sin(Angle) * Axis.xyz.y;
		Mat.matrix[2][1] = (1 - cos(Angle)) * Axis.xyz.z * Axis.xyz.y + sin(Angle) * Axis.xyz.x;
		Mat.matrix[2][2] = cos(Angle) + (1 - cos(Angle)) * pow(Axis.xyz.z, 2.f);
	}
};

struct Polygonn {
	vector<Point> plane;

	Polygonn() : plane() {}
	Polygonn(vector<Point> plane) : plane(plane) {}
};

struct Object {
	vector<Polygonn> polygons;

	Object() : polygons() {}
	Object(vector<Polygonn> polygons) : polygons(polygons) {}

	void Rotation(Matrix matrix) {
		for (Polygonn &RotatePolygon : polygons) {
			for (Point &RotatePoint : RotatePolygon.plane) {
				multiplie(RotatePoint, matrix);
			}
		}
	}
};

struct ViewPort {
	float width, height, deep;

	ViewPort() : width(1.f), height(1.f), deep(1.f) {}
};

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
		for (Polygonn WorldPolygon : WorldObject.polygons) {
			Polygonn ViewPortPolygon;
			for (Point WorldPoint : WorldPolygon.plane) {
				Point ViewPortPoint;

				ViewPortPoint.x = (WorldPoint.x - this->position.x) * this->viewport.deep / (WorldPoint.z - this->position.z);
				ViewPortPoint.y = (WorldPoint.y - this->position.y) * this->viewport.deep / (WorldPoint.z - this->position.z);
				ViewPortPoint.z = this->viewport.deep;

				ViewPortPolygon.plane.push_back(ViewPortPoint);
			}
			ViewPortObject.polygons.push_back(ViewPortPolygon);
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

	void Start(HANDLE &buffer) {
		for (Object WorldObject : scene.scene) {
			this->PrintPolygon(WorldObject, this->camera);
			this->FillBuffer(buffer);
		}
	}

	Vector GetNormale(Polygonn &WorldPolygon) {
		Vector vec1(WorldPolygon.plane[1] - WorldPolygon.plane[0]);
		Vector vec2(WorldPolygon.plane[2] - WorldPolygon.plane[0]);
		Vector Normal = Vector::Cross(vec2, vec1);

		if (Normal.Length() < 0.00001f) {
			Vector vec3(WorldPolygon.plane[2] - WorldPolygon.plane[0]);
			Vector vec4(WorldPolygon.plane[3] - WorldPolygon.plane[0]);
			Normal = Vector::Cross(vec4, vec3);
		}

		Normal = Normal.Normalize();
		return Normal;
	}

	void PrintPolygon(Object WorldObject, Camera Cam) {
		for (Polygonn WorldPolygon : WorldObject.polygons) {
			if (!this->settings.RenderInvisiblePolygons) {
				Vector Normal = this->GetNormale(WorldPolygon);
				Vector ViewCam(this->camera.position - WorldPolygon.plane[0]);
				
				float DotProduct = Vector::Dot(Normal, ViewCam);
				if (DotProduct > 0.f)
					continue;
			}

			Polygonn BufferPolygon;
			for (Point WorldPoint : WorldPolygon.plane) {
				//WORLDPOINT TO VIEWPORTPOINT
				Point ViewPortPoint;
				ViewPortPoint.x = (WorldPoint.x - Cam.position.x) * Cam.viewport.deep / (WorldPoint.z - Cam.position.z);
				ViewPortPoint.y = (WorldPoint.y - Cam.position.y) * Cam.viewport.deep / (WorldPoint.z - Cam.position.z);
				ViewPortPoint.z = Cam.viewport.deep;

				//VIEWPORTPOINT TO SCREENPOINT
				ViewPortPoint.x *= Screen::AspectRatio * Screen::PixelRatio;
				Point BufferPoint;
				BufferPoint.x = Screen::width * (ViewPortPoint.x + Cam.viewport.width / 2.f);
				BufferPoint.y = Screen::height * (-ViewPortPoint.y + Cam.viewport.height / 2.f);

				//PUSH TO SCREENPOINT TO SCREENPOLYGON
				BufferPolygon.plane.push_back(BufferPoint);
			}

			int BPS = BufferPolygon.plane.size();
			for (int i = 0; i < BPS; i++) {
				Point ScreenPoint1 = BufferPolygon.plane[i];
				Point ScreenPoint2 = BufferPolygon.plane[(i + 1) % BPS];

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

	void FillBuffer(HANDLE &buffer) {
		DWORD useless = NULL;
		WriteConsoleOutputCharacterW(buffer, Screen::screen, Screen::width * Screen::height, { 0, 0 }, &useless);
	}
};

void multiplie(Point &point, Matrix rotate) {
	Point Newpoint;
	Newpoint.x = rotate.matrix[0][0] * point.x + rotate.matrix[0][1] * point.y + rotate.matrix[0][2] * point.z;
	Newpoint.y = rotate.matrix[1][0] * point.x + rotate.matrix[1][1] * point.y + rotate.matrix[1][2] * point.z;
	Newpoint.z = rotate.matrix[2][0] * point.x + rotate.matrix[2][1] * point.y + rotate.matrix[2][2] * point.z;

	point.x = Newpoint.x;
	point.y = Newpoint.y;
	point.z = Newpoint.z;
}

vector<Polygonn> generateSphere(float radius, int rings, int sectors) {
	vector<Polygonn> spherePolys;
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
			spherePolys.push_back(Polygonn(vector<Point>{
				Point(x3, y3, z1),  // Верх-Лево
				Point(x2, y2, z1), // Верх-Право
				Point(x1, y1, z0), // Низ-Право
				Point(x0, y0, z0), // Низ-Лево
			}));
		}
	}
	return spherePolys;
}

std::vector<Polygonn> generateTorus(float radius, float tubeRadius, int segments, int sides) {
	std::vector<Polygonn> torusPolys;
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
			torusPolys.push_back(Polygonn(std::vector<Point>{
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
	vector<Polygonn> cube = {
		Polygonn(vector<Point> {Point(-0.5f, -0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f), Point( 0.5f,  0.5f, -0.5f), Point( 0.5f, -0.5f, -0.5f)}), //front
		Polygonn(vector<Point> {Point( 0.5f,  0.5f, -0.5f), Point( 0.5f,  0.5f,  0.5f), Point( 0.5f, -0.5f,  0.5f), Point( 0.5f, -0.5f, -0.5f)}), //right
		Polygonn(vector<Point> {Point(-0.5f, -0.5f,  0.5f), Point( 0.5f, -0.5f,  0.5f), Point( 0.5f,  0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //back
		Polygonn(vector<Point> {Point(-0.5f,  0.5f, -0.5f), Point(-0.5f, -0.5f, -0.5f), Point(-0.5f, -0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //left
		Polygonn(vector<Point> {Point(-0.5f,  0.5f,  0.5f), Point( 0.5f,  0.5f,  0.5f), Point( 0.5f,  0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f)}), //top
		Polygonn(vector<Point> {Point(-0.5f, -0.5f, -0.5f), Point( 0.5f, -0.5f, -0.5f), Point( 0.5f, -0.5f,  0.5f), Point(-0.5f, -0.5f,  0.5f)}), //bottom
	};
	Object Cube(cube);

	std::vector<Polygonn> torus = generateTorus(1.5f, 0.33f, 30, 30);
	vector<Polygonn> sphereData = generateSphere(0.5f, 12, 12);
	vector<Polygonn> sphereData1 = generateSphere(0.50f, 6, 6);
	Scene MScene({torus, sphereData, cube});
	//Scene MScene({Cube});

	Camera MCamera(Point(0.f, 0.f, -6.f), Vector(0.f, 0.f, 1.f), ViewPort());

	//Sleep(4000);
	Matrix mat, matx, maty, matz;
	Vector axis(Point(300.f, 1000.f, 650.f));
	axis.Normalize(axis);
	Matrix::CreateRotateMatrix(mat, axis, 3.f);
	Matrix::CreateRotateMatrix(matx, Vector(0.f, 1.f, 0.f), 4.f);
	Matrix::CreateRotateMatrix(maty, Vector(1.f, 0.f, 0.f), 2.f);
	Matrix::CreateRotateMatrix(matz, Vector(1.f, 0.33f, 1.f).Normalize(), 2.f);
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