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

struct Render {
	Scene scene;
	Camera camera;

	

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

	void Start(wchar_t* buffer, const int& width, const int& height, const float& aspectRatio, const float& pixelRatio) const;

	Vector GetNormale(const Console3D::Polygon& WorldPolygon) const;

	void PrintPolygon(const Object WorldObject, wchar_t* buffer, const int& width, const int& height, const float& aspectRatio, const float& pixelRatio) const;

	void PrintLine(Point p1, Point p2, wchar_t* buffer, const int& width, const int& height) const;

	void PolygonFilling(const Console3D::Polygon& bufferPolygon, wchar_t* buffer, const int& width, const int& height) const;

	std::vector<Point> Interpolate(Point p1, Point p2, const int& width, const int& height) const;

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