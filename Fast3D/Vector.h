#pragma once
#include <iostream>
#include "Point.h"
#include "Matrix.h"

struct Matrix;

struct Vector  {
	Point direction;

	Vector();
	Vector(const float same);
	Vector(const float x, const float y, const float z);
	Vector(const Point direction);

	Vector& operator=  (const Vector& other);
	bool    operator== (const Vector& other) const;
	bool    operator!= (const Vector& other) const;
	Vector  operator-  () const;

	Vector  operator+  (const Vector& other) const;
	Vector& operator+= (const Vector& other);
	Vector  operator-  (const Vector& other) const;
	Vector& operator-= (const Vector& other);

	Vector  operator*  (const Matrix& Mat) const;
	Vector& operator*= (const Matrix& Mat);

	float   Length() const;
	Vector  Normalized() const;
	Vector& Normalize();

	static float  Dot(const Vector& vec1, const Vector& vec2);
	static Vector Cross(const Vector& vec1, const Vector& vec2);
};