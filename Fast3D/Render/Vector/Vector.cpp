#include "Vector.h"
using namespace Console3D;

Vector::Vector() : direction(0.f, 0.f, 1.f) {};
Vector::Vector(const float same) : direction(same, same, same) {};
Vector::Vector(const float x, const float y, const float z) : direction(x, y, z) {};
Vector::Vector(const Point direction) : direction(direction) {}

Vector& Vector::operator=(const Vector& other)
{
	this->direction = other.direction;
	return *this;
}

bool Vector::operator==(const Vector& other) const {
	return (this->direction == other.direction);
}

bool Vector::operator!=(const Vector& other) const {
	return !(*this == other);
}

Vector Vector::operator-() const {
	return Vector(-this->direction);
}

Vector Vector::operator+(const Vector& other) const
{
	return Vector(this->direction + other.direction);
}

Vector& Vector::operator+=(const Vector& other)
{
	*this += other;
	return *this;
}

Vector Vector::operator-(const Vector& other) const
{
	return Vector(this->direction - other.direction);
}

Vector& Vector::operator-=(const Vector& other)
{
	*this -= other;
	return *this;
}

Vector Vector::operator*(const Matrix& Mat) const
{
	Vector Vec;
	Vec.direction.x = this->direction.x * Mat.data[0][0] + this->direction.y * Mat.data[0][1] + this->direction.z * Mat.data[0][2];
	Vec.direction.y = this->direction.x * Mat.data[1][0] + this->direction.y * Mat.data[1][1] + this->direction.z * Mat.data[1][2];
	Vec.direction.z = this->direction.x * Mat.data[2][0] + this->direction.y * Mat.data[2][1] + this->direction.z * Mat.data[2][2];
	
	return Vec;
}

Vector& Vector::operator*=(const Matrix& Mat)
{
	*this = *this * Mat;
	return *this;
}


float Vector::Length() const {
	return sqrt(pow(this->direction.x, 2.f) + pow(this->direction.y, 2.f) + pow(this->direction.z, 2.f));
}

Vector Vector::Normalized() const {
	float length = this->Length();
	return Vector(this->direction.x / length, this->direction.y / length, this->direction.z / length);
}

Vector& Vector::Normalize() {
	float length = this->Length();
	this->direction /= length;
	return *this;
}

float Vector::Dot(const Vector& vec1, const Vector& vec2) {
	return vec1.direction.x * vec2.direction.x + vec1.direction.y * vec2.direction.y + vec1.direction.z * vec2.direction.z;
}

Vector Vector::Cross(const Vector& vec1, const Vector& vec2) {
	return Vector(
		vec1.direction.y * vec2.direction.z - vec1.direction.z * vec2.direction.y,
		vec1.direction.z * vec2.direction.x - vec1.direction.x * vec2.direction.z,
		vec1.direction.x * vec2.direction.y - vec1.direction.y * vec2.direction.x
	);
}