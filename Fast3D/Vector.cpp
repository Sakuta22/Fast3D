#include "Vector.h"

Vector::Vector() : direction(0.f, 0.f, 1.f) {};
Vector::Vector(float same) : direction(same, same, same) {};
Vector::Vector(float x, float y, float z) : direction(x, y, z) {};
Vector::Vector(Point direction) : direction(direction) {};

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