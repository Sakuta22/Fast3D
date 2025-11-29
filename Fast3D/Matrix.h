#pragma once
#include "Vector.h"

const float M_PI = 3.14159265358979323846;

struct Vector;

struct Matrix {
	float data[3][3];

	Matrix();

	static void   CreateRotateMatrix(Matrix& mat, const Vector& axis, float angle);
	static Matrix RotateMatrix(const Vector& axis, float angle);
};