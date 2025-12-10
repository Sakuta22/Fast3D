#include "Point.h"
using namespace Console3D;

Point::Point() : x(0.f), y(0.f), z(0.f) {};
Point::Point(const float same) : x(same), y(same), z(same) {};
Point::Point(const float x, const float y, const float z) : x(x), y(y), z(z) {};

Point& Point::operator=(const Point& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

bool Point::operator==(const Point& other) const {
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}

bool Point::operator!=(const Point& other) const {
	return !(*this == other);
}

Point Point::operator-() const {
	return Point(-this->x, -this->y, -this->z);
}

Point Point::operator+(const Point& other) const
{
	return Point(this->x + other.x, this->y + other.y, this->z + other.z);
}

Point& Point::operator+=(const Point& other)
{
	*this = *this + other;
	return *this;
}

Point Point::operator-(const Point& other) const
{
	return Point(this->x - other.x, this->y - other.y, this->z - other.z);
}

Point& Point::operator-=(const Point& other)
{
	*this = *this - other;
	return *this;
}

Point Point::operator+(const float& scalar) const
{
	return Point(this->x + scalar, this->y + scalar, this->z + scalar);
}

Point& Point::operator+=(const float& scalar)
{
	*this = *this + scalar;
	return *this;
}

Point Point::operator-(const float& scalar) const
{
	return Point(this->x - scalar, this->y - scalar, this->z - scalar);
}

Point& Point::operator-=(const float& scalar)
{
	*this = *this - scalar;
	return *this;
}

Point Point::operator*(const float& scalar) const
{
	return Point(this->x * scalar, this->y * scalar, this->z * scalar);
}

Point& Point::operator*=(const float& scalar)
{
	*this = *this * scalar;
	return *this;
}

Point Point::operator/(const float& scalar) const
{
	return Point(this->x / scalar, this->y / scalar, this->z / scalar);
}

Point& Point::operator/=(const float& scalar)
{
	*this = *this / scalar;
	return *this;
}