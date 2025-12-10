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

	Render(Scene scene, Camera camera);

	void Start(HANDLE& buffer) const;

	Vector GetNormale(const Console3D::Polygon& WorldPolygon) const;

	void PrintPolygon(const Object WorldObject) const;

	void PrintLine(Point p1, Point p2) const;

	void FillBuffer(HANDLE& buffer) const;

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