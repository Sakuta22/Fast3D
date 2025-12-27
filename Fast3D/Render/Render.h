#pragma once
#include <Windows.h>	
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Matrix/Matrix.h"
#include "Polygon/Polygon.h"
#include "Primitives/Primitives.h"
#include "Object/Object.h"
#include "ViewPort/ViewPort.h"
#include "Camera/Camera.h"
#include "Scene/Scene.h"

#include "../Screen/Screen.h"

namespace Console3D {

struct BaseScreen {
	wchar_t* data;
	int width;
	int height;
	float aspectRatio;
	float pixelRatio;

	void UpdateRatio() {
		this->aspectRatio = (float)this->height / this->width;
		this->pixelRatio = 24.f / 11.f;
	}
};

struct Render {
	Scene scene;
	Camera camera;
	BaseScreen screen;

	//struct ZBuffer { //Hidden Surface Removal
	//	float* fBuffer;
	//	float* zBuffer;
	//	float size;

	//	ZBuffer(wchar_t& frameBuffer, float& size) : size(size) {
	//		fBuffer = frameBuffer;
	//		zBuffer = new float[size];
	//	}


	//};
	//ZBuffer zBuffer;

	Render(Scene scene, Camera camera);

	void SetScreen(BaseScreen screen);

	void Start() const;

	Vector GetNormale(const Console3D::Polygon& WorldPolygon) const;

	void PrintPolygon(const Object WorldObject) const;

	void PrintLine(Point p1, Point p2) const;

	void PolygonFilling(const Console3D::Polygon& bufferPolygon) const;

	std::vector<Point> Interpolate(Point p1, Point p2) const;

	enum class CullMode {
		None,
		Front,
		Back
	};
	enum class WindingOrder {
		Clockwise,
		CounterClockwise
	};
//private:
	struct Settings {
		Render::CullMode cullMode = Render::CullMode::Back;
		Render::WindingOrder windingOrder = Render::WindingOrder::Clockwise;
	};

	Settings settings;
};

}