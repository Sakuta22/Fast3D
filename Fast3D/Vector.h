#pragma once
#include <iostream>
#include "Point.h"

struct Vector  {
	Point direction;

	Vector();
	Vector(float same);
	Vector(float x, float y, float z);
	Vector(Point direction);

	float   Length() const;
	Vector  Normalized() const;
	Vector& Normalize();

	static float  Dot(const Vector& vec1, const Vector& vec2);
	static Vector Cross(const Vector& vec1, const Vector& vec2);
};