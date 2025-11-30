#pragma once
#include <Windows.h>	
#include "Point/Point.h"
#include "Vector/Vector.h"
#include "Matrix/Matrix.h"
#include "Polygon/Polygon.h"
#include "Object/Object.h"
#include "ViewPort/ViewPort.h"
#include "Camera/Camera.h"
#include "Scene/Scene.h"

#include "../Screen/Screen.h"

struct Render {
	static struct Settings {
		static bool RenderInvisiblePolygons;
	};

	Scene scene;
	Camera camera;

	Render(Scene scene, Camera camera);

	void Start(HANDLE& buffer) const;

	Vector GetNormale(const Fast3d::Polygon& WorldPolygon) const;

	void PrintPolygon(const Object WorldObject) const;

	void PrintLine(Point p1, Point p2) const;

	void FillBuffer(HANDLE& buffer) const;
};