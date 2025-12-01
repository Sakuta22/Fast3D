#include "Render.h"
using namespace Console3D;

Render::Render(Scene scene, Camera camera) : scene(scene), camera(camera) {}

void Render::Start(HANDLE& buffer) const {
	for (Object WorldObject : scene.data) {
		this->PrintPolygon(WorldObject);
		this->FillBuffer(buffer);
	}
}

Vector Render::GetNormale(const Console3D::Polygon& WorldPolygon) const {
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

void Render::PrintPolygon(const Object WorldObject) const {
	for (Console3D::Polygon WorldPolygon : WorldObject.data) {
		if (!Render::Settings::RenderInvisiblePolygons) {
			Vector Normal = this->GetNormale(WorldPolygon);
			Vector ViewCam(Vector(this->camera.position) - WorldPolygon.data[0]);

			float DotProduct = Vector::Dot(Normal, ViewCam);
			if (DotProduct > 0.f)
				continue;
		}

		Console3D::Polygon BufferPolygon;
		for (Vector WorldVector : WorldPolygon.data) {
			//WORLDVECTOR TO VIEWPORTVECTOR
			Vector ViewPortVector;
			ViewPortVector.direction.x = (WorldVector.direction.x - this->camera.position.x) * this->camera.viewport.deep / (WorldVector.direction.z - this->camera.position.z);
			ViewPortVector.direction.y = (WorldVector.direction.y - this->camera.position.y) * this->camera.viewport.deep / (WorldVector.direction.z - this->camera.position.z);
			ViewPortVector.direction.z = this->camera.viewport.deep;

			//VIEWPORTVECTOR TO BUFFERPOINT
			ViewPortVector.direction.x *= Screen::AspectRatio * Screen::PixelRatio;
			Point BufferPoint;
			BufferPoint.x = Screen::width * (ViewPortVector.direction.x + this->camera.viewport.width / 2.f);
			BufferPoint.y = Screen::height * (-ViewPortVector.direction.y + this->camera.viewport.height / 2.f);

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

void Render::PrintLine(Point p1, Point p2) const {
	if (p1.y > p2.y)
		std::swap(p1, p2);

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

void Render::FillBuffer(HANDLE& buffer) const {
	DWORD useless = NULL;
	WriteConsoleOutputCharacterW(buffer, Screen::screen, Screen::width * Screen::height, { 0, 0 }, &useless);
}

bool Render::Settings::RenderInvisiblePolygons = false;