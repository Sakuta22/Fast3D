#include "Render.h"
using namespace Console3D;

Render::Render(Scene scene, Camera camera) : scene(scene), camera(camera) {}

void Console3D::Render::SetScreen(BaseScreen screen)
{
	this->screen = screen;
	this->zBuffer.SetZBuffer(&this->screen);
}

void Render::Start() const {
	

	for (Object WorldObject : scene.data) {
		this->PrintPolygon(WorldObject);
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

void Render::PrintPolygon(const Object WorldObject) const {
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
				break;
			case Render::CullMode::Back:
				if (DotProduct > 0.f)
					continue;
				break;
			}
		}

		Console3D::Polygon BufferPolygon;
		for (Vector WorldVector : WorldPolygon.data) {
			//WORLDVECTOR TO VIEWPORTVECTOR
			Vector ViewPortVector;
			ViewPortVector.direction.x = (WorldVector.direction.x - this->camera.position.x) * this->camera.viewport.deep / (WorldVector.direction.z - this->camera.position.z);
			ViewPortVector.direction.y = (WorldVector.direction.y - this->camera.position.y) * this->camera.viewport.deep / (WorldVector.direction.z - this->camera.position.z);
			ViewPortVector.direction.z = 1.f / (WorldVector.direction.z - this->camera.position.z);

			//VIEWPORTVECTOR TO BUFFERPOINT
			ViewPortVector.direction.x *= this->screen.aspectRatio * this->screen.pixelRatio;
			Point BufferPoint;
			BufferPoint.x = this->screen.width * (ViewPortVector.direction.x + this->camera.viewport.width / 2.f);
			BufferPoint.y = this->screen.height * (-ViewPortVector.direction.y + this->camera.viewport.height / 2.f);
			BufferPoint.z = ViewPortVector.direction.z;

			//PUSH TO BUFFERNPOINT TO BUFFERPOLYGON
			BufferPolygon.data.push_back(BufferPoint);
		}

		this->PolygonFilling(BufferPolygon, WorldObject.colorFill);

		/*int BPS = BufferPolygon.data.size();
		for (int i = 0; i < BPS; i++) {
			Point ScreenPoint1 = BufferPolygon.data[i].direction;
			Point ScreenPoint2 = BufferPolygon.data[(i + 1) % BPS].direction;

			PrintLine(ScreenPoint1, ScreenPoint2, WorldObject.colorStroke);
		}*/
	}
}

void Render::PrintLine(Point p1, Point p2, wchar_t color) const {
	if (p1.y > p2.y)
		std::swap(p1, p2);

	if (abs(p2.x - p1.x) > abs(p2.y - p1.y)) {
		p1.x = round(p1.x), p2.x = round(p2.x);
		float y = p1.y, dy = abs((p2.y - p1.y) / (p2.x - p1.x));
		float z = p1.z, dz = (p2.z - p1.z) / abs(p2.x - p1.x);
		if (p1.x <= p2.x) {
			for (int x = p1.x; x <= p2.x; x++, y += dy, z += dz) {
				if ((0 <= x && x < this->screen.width) && (0 <= round(y) && round(y) < this->screen.height)) {
					//this->screen.data[(int)(this->screen.width * round(y) + x)] = color;

					if (this->zBuffer.zBuffer[(int)(this->screen.width * round(y) + x)] < z) {
						this->zBuffer.zBuffer[(int)(this->screen.width * round(y) + x)] = z;
						this->zBuffer.fBuffer->data[(int)(this->screen.width * round(y) + x)] = color;
					}
				}
			}
		}
		else {
			for (int x = p1.x; x >= p2.x; x--, y += dy, z += dz) {
				if ((0 <= x && x < this->screen.width) && (0 <= round(y) && round(y) < this->screen.height)) {
					//this->screen.data[(int)(this->screen.width * round(y) + x)] = color;

					if (this->zBuffer.zBuffer[(int)(this->screen.width * round(y) + x)] < z) {
						this->zBuffer.zBuffer[(int)(this->screen.width * round(y) + x)] = z;
						this->zBuffer.fBuffer->data[(int)(this->screen.width * round(y) + x)] = color;
					}
				}
			}
		}
	}
	else {
		p1.y = round(p1.y), p2.y = round(p2.y);
		float x = p1.x, dx = (p2.y - p1.y == 0.f ? 0.f : (p2.x - p1.x) / (p2.y - p1.y));
		float z = p1.z, dz = (p2.z - p1.z) / abs(p2.y - p1.y);
		for (int y = p1.y; y <= p2.y; y++, x += dx) {
			if ((0 <= round(x) && round(x) < this->screen.width) && (0 <= y && y < this->screen.height)) {
				//this->screen.data[(int)(this->screen.width * y + round(x))] = color;

				if (this->zBuffer.zBuffer[(int)(this->screen.width * y + round(x))] < z) {
					this->zBuffer.zBuffer[(int)(this->screen.width * y + round(x))] = z;
					this->zBuffer.fBuffer->data[(int)(this->screen.width * y + round(x))] = color;
				}
			}
		}
	}
}

void Render::PolygonFilling(const Console3D::Polygon& bufferPolygon, wchar_t color) const
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

		std::vector<Point> long13 = Interpolate(p1, p3);
		std::vector<Point> short1 = Interpolate(p1, p2);
		std::vector<Point> short2 = Interpolate(p2, p3);

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
			float z = long13[i].z, dz = -(short1[i].z - z) / (xf - xs);
			if (xs == long13[i].x)
				z = long13[i].z, dz = (xf == xs ? 0.f : (short1[i].z - z) / (float)(xf - xs));
			else
				z = short1[i].z, dz = (xf == xs ? 0.f : (long13[i].z - z) / (float)(xf - xs));
				
			for (int x = xs; x <= xf; x++, z += dz) {
				if ((0 <= x && x < this->screen.width) && (0 <= y && y < this->screen.height)) {
					//this->screen.data[(int)(this->screen.width * y + x)] = color;

					if (this->zBuffer.zBuffer[(int)(this->screen.width * y + x)] < z) {
						this->zBuffer.zBuffer[(int)(this->screen.width * y + x)] = z;
						this->zBuffer.fBuffer->data[(int)(this->screen.width * y + x)] = color;
					}
				}
			}
		}
	}
}

std::vector<Point> Console3D::Render::Interpolate(Point p1, Point p2) const
{
	std::vector<Point> interpolate;

	p1.y = round(p1.y), p2.y = round(p2.y);
	float x = p1.x, dx = (p2.y - p1.y == 0.f ? 0.f : (p2.x - p1.x) / (p2.y - p1.y));
	float z = p1.z, dz = (p2.z - p1.z) / abs(p2.y - p1.y);
	for (int y = p1.y; y <= p2.y; y++, x += dx, z += dz) {
		interpolate.push_back(Point(round(x), y, z));
	}

	return interpolate;
}

std::vector<Point> Console3D::Render::Interpolate2(float i0, float d0, float i1, float d1)
{
	std::vector<Point> values;

	float step = (d1 - d0) / (i1 - i0);
	float d = d0;
	for (i0; i0 <= i1; i0++) {
		values.push_back(d);
		d += step;
	}

	return values;
}
