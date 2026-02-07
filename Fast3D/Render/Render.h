#pragma once
#include <iostream>
#include <sstream>
#include <Windows.h>	
#include <chrono>

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

struct ZBuffer { //Hidden Surface Removal
	BaseScreen* fBuffer;
	float* zBuffer;

	ZBuffer() : fBuffer(nullptr), zBuffer(nullptr) {};

	ZBuffer(BaseScreen* frameBuffer) : zBuffer(nullptr) {
		this->SetZBuffer(frameBuffer);
	}

	void SetZBuffer(BaseScreen* frameBuffer) {
		this->fBuffer = frameBuffer;

		if (this->zBuffer != nullptr)
			delete[] this->zBuffer;
		zBuffer = new float[this->fBuffer->width * this->fBuffer->height];
		std::fill(zBuffer, zBuffer + this->fBuffer->width * this->fBuffer->height, 0.f);
	}

	~ZBuffer() {
		fBuffer = nullptr;
		delete[] zBuffer;
	}
};

struct TickSystem {
	float tickRate;
	float accumulator;
	std::chrono::steady_clock::time_point timeThen;

	TickSystem(float tickRate = 1.f / 60.f) : tickRate(tickRate), accumulator(0.f) {
		this->Restart();
	};

	void Restart() {
		this->timeThen = std::chrono::steady_clock::now();
	}

	float Update() {
		auto timeNow = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration<float>(timeNow - this->timeThen).count();
		this->timeThen = timeNow;

		this->accumulator += deltaTime;
		this->accumulator = min(this->accumulator, 0.25f);

		return deltaTime;
	}

	bool NeedATick() {
		if (this->accumulator >= this->tickRate) {
			this->accumulator -= this->tickRate;
			return true;
		}

		return false;
	}
};

//struct DebugWindow {
//	///
//
//	void PrintInfo() {
//		std::wstringstream wss;
//		wss << 
//	}
//};

struct Render {
	Scene scene;
	Camera camera;
	BaseScreen screen;
	ZBuffer zBuffer;

	//TickSystem tickSystem;

	Render(Scene scene, Camera camera);

	void SetScreen(BaseScreen screen);

	void Start() const;

	Vector GetNormale(const Console3D::Polygon& WorldPolygon) const;

	void PrintPolygon(const Object WorldObject) const;

	void PrintLine(Point p1, Point p2, wchar_t color) const;

	void PolygonFilling(const Console3D::Polygon& bufferPolygon, wchar_t color) const;

	std::vector<Point> Interpolate(Point p1, Point p2) const;
	static std::vector<Point> Interpolate2(float i0, float d0, float i1, float d1);

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