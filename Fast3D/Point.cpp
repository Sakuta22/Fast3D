#include "Point.h"

Point::Point() : x(0.f), y(0.f), z(0.f) {};
Point::Point(float same) : x(same), y(same), z(same) {};
Point::Point(float x, float y, float z) : x(x), y(y), z(z) {};

Point Point::operator- (Point& other) const {
	return Point(this->x - other.x, this->y - other.y, this->z - other.z);
}

bool Point::operator== (Point& other) const {
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}