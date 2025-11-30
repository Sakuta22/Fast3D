#include "Matrix.h"

Matrix::Matrix() : data{ { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f } } {};

void Matrix::CreateRotateMatrix(Matrix& mat, const Vector& axis, float angle) {
	angle *= M_PI / 180.f;

	mat.data[0][0] = cos(angle) + (1 - cos(angle)) * pow(axis.direction.x, 2.f);
	mat.data[0][1] = (1 - cos(angle)) * axis.direction.x * axis.direction.y - sin(angle) * axis.direction.z;
	mat.data[0][2] = (1 - cos(angle)) * axis.direction.x * axis.direction.z + sin(angle) * axis.direction.y;

	mat.data[1][0] = (1 - cos(angle)) * axis.direction.y * axis.direction.x + sin(angle) * axis.direction.z;
	mat.data[1][1] = cos(angle) + (1 - cos(angle)) * pow(axis.direction.y, 2.f);
	mat.data[1][2] = (1 - cos(angle)) * axis.direction.y * axis.direction.z - sin(angle) * axis.direction.x;

	mat.data[2][0] = (1 - cos(angle)) * axis.direction.z * axis.direction.x - sin(angle) * axis.direction.y;
	mat.data[2][1] = (1 - cos(angle)) * axis.direction.z * axis.direction.y + sin(angle) * axis.direction.x;
	mat.data[2][2] = cos(angle) + (1 - cos(angle)) * pow(axis.direction.z, 2.f);
}

Matrix Matrix::RotateMatrix(const Vector& axis, float angle)
{
	Matrix mat;

	Matrix::CreateRotateMatrix(mat, axis, angle);

	return mat;
}
