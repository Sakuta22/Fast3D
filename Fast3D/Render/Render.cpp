#include "Render.h"
using namespace Console3D;

Render::Render(Scene scene, Camera camera) : scene(scene), camera(camera) {}

void Render::Start(wchar_t* buffer, const int& width, const int& height, const float& aspectRatio, const float& pixelRatio) const {
	//this->zBuffer.Update(buffer, Screen::width * Screen::height);
	
	for (Object WorldObject : scene.data) {
		this->PrintPolygon(WorldObject, buffer, width, height, aspectRatio, pixelRatio);
	}
}

Vector Render::GetNormale(const Console3D::Polygon& WorldPolygon) const {
	Vector vec1(WorldPolygon.data[1] - WorldPolygon.data[0]);
	Vector vec2(WorldPolygon.data[2] - WorldPolygon.data[0]);
	Vector Normal;
	switch (this->settings.windingOrder)
	{
	case Render::WindingOrder::Clockwise:
		Normal = Vector::Cross(vec2, vec1);
		break;
	case Render::WindingOrder::CounterClockwise:
		Normal = Vector::Cross(vec1, vec2);
		break;
	}

	if (Normal.Length() < 0.00001f) {
		Vector vec3(WorldPolygon.data[2] - WorldPolygon.data[0]);
		Vector vec4(WorldPolygon.data[3] - WorldPolygon.data[0]);
		switch (this->settings.windingOrder)
		{
		case Render::WindingOrder::Clockwise:
			Normal = Vector::Cross(vec4, vec3);
			break;
		case Render::WindingOrder::CounterClockwise:
			Normal = Vector::Cross(vec3, vec4);
			break;
		}
	}

	return Normal.Normalized();
}

void Render::PrintPolygon(const Object WorldObject, wchar_t* buffer, const int& width, const int& height, const float& aspectRatio, const float& pixelRatio) const {
	for (Console3D::Polygon WorldPolygon : WorldObject.data) {
		if (this->settings.cullMode != Render::CullMode::None) {
			Vector Normal = this->GetNormale(WorldPolygon);
			Vector ViewCam(Vector(this->camera.position) - WorldPolygon.data[0]);

			float DotProduct = Vector::Dot(Normal, ViewCam);
			switch (this->settings.cullMode)
			{
			case Render::CullMode::Front:
				if (DotProduct < 0.f)
					continue;
			case Render::CullMode::Back:
				if (DotProduct > 0.f)
					continue;
			}
		}

		Console3D::Polygon BufferPolygon;
		for (Vector WorldVector : WorldPolygon.data) {
			//WORLDVECTOR TO VIEWPORTVECTOR
			Vector ViewPortVector;
			ViewPortVector.direction.x = (WorldVector.direction.x - this->camera.position.x) * this->camera.viewport.deep / (WorldVector.direction.z - this->camera.position.z);
			ViewPortVector.direction.y = (WorldVector.direction.y - this->camera.position.y) * this->camera.viewport.deep / (WorldVector.direction.z - this->camera.position.z);
			ViewPortVector.direction.z = this->camera.viewport.deep;

			//VIEWPORTVECTOR TO BUFFERPOINT
			ViewPortVector.direction.x *= aspectRatio * pixelRatio;
			Point BufferPoint;
			BufferPoint.x = width * (ViewPortVector.direction.x + this->camera.viewport.width / 2.f);
			BufferPoint.y = height * (-ViewPortVector.direction.y + this->camera.viewport.height / 2.f);

			//PUSH TO BUFFERNPOINT TO BUFFERPOLYGON
			BufferPolygon.data.push_back(BufferPoint);
		}

		this->PolygonFilling(BufferPolygon, buffer, width, height);

		int BPS = BufferPolygon.data.size();
		for (int i = 0; i < BPS; i++) {
			Point ScreenPoint1 = BufferPolygon.data[i].direction;
			Point ScreenPoint2 = BufferPolygon.data[(i + 1) % BPS].direction;

			PrintLine(ScreenPoint1, ScreenPoint2, buffer, width, height);
		}
	}
}

void Render::PrintLine(Point p1, Point p2, wchar_t* buffer, const int& width, const int& height) const {
	if (p1.y > p2.y)
		std::swap(p1, p2);

	if (abs(p2.x - p1.x) > abs(p2.y - p1.y)) {
		p1.x = round(p1.x), p2.x = round(p2.x);
		float y = p1.y, dy = abs((p2.y - p1.y) / (p2.x - p1.x));
		if (p1.x <= p2.x) {
			for (int x = p1.x; x <= p2.x; x++, y += dy) {
				if ((0 <= x && x < width) && (0 <= round(y) && round(y) < height))
					buffer[(int)(width * round(y) + x)] = '*';
			}
		}
		else {
			for (int x = p1.x; x >= p2.x; x--, y += dy) {
				if ((0 <= x && x < width) && (0 <= round(y) && round(y) < height))
					buffer[(int)(width * round(y) + x)] = '*';
			}
		}
	}
	else {
		p1.y = round(p1.y), p2.y = round(p2.y);
		float x = p1.x, dx = (p2.y - p1.y == 0.f ? 0.f : (p2.x - p1.x) / (p2.y - p1.y));
		for (int y = p1.y; y <= p2.y; y++, x += dx) {
			if ((0 <= round(x) && round(x) < width) && (0 <= y && y < height))
				buffer[(int)(width * y + round(x))] = '*';
		}
	}
}

void Render::PolygonFilling(const Console3D::Polygon& bufferPolygon, wchar_t* buffer, const int& width, const int& height) const
{
	int BPS = bufferPolygon.data.size();
	for (int i = 0; i < BPS - 1; i += 2)
	{
		Point p1 = bufferPolygon.data[i].direction;
		Point p2 = bufferPolygon.data[i + 1].direction;
		Point p3 = bufferPolygon.data[(i + 2) % BPS].direction;

		if (p2.y < p1.y)
			std::swap(p2, p1);
		if (p3.y < p1.y)
			std::swap(p3, p1);
		if (p3.y < p2.y)
			std::swap(p3, p2);

		std::vector<Point> long13 = Interpolate(p1, p3, width, height);
		std::vector<Point> short1 = Interpolate(p1, p2, width, height);
		std::vector<Point> short2 = Interpolate(p2, p3, width, height);

		if (!short1.empty())
			short1.pop_back();
		for (Point p : short2)
			short1.push_back(p);

		if (long13.size() != short1.size())
			exit(0);

		for (int i = 0; i < long13.size(); i++) {
			int y = long13[i].y;
			int xs = min(long13[i].x, short1[i].x);
			int xf = max(long13[i].x, short1[i].x);
			for (int x = xs; x <= xf; x++) {
				if ((0 <= x && x < width) && (0 <= y && y < height)) {
					buffer[(int)(width * y + x)] = 'g';
				}
			}
		}
	}
}

std::vector<Point> Console3D::Render::Interpolate(Point p1, Point p2, const int& width, const int& height) const
{
	std::vector<Point> interpolate;

	p1.y = round(p1.y), p2.y = round(p2.y);
	float x = p1.x, dx = (p2.y - p1.y == 0.f ? 0.f : (p2.x - p1.x) / (p2.y - p1.y));
	for (int y = p1.y; y <= p2.y; y++, x += dx) {
		interpolate.push_back(Point(round(x), y, 0));
	}

	return interpolate;
}
