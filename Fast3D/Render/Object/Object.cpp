#include "Object.h"

Object::Object() : data() {};
Object::Object(const std::vector<Fast3d::Polygon> data) : data(data) {};

void Object::Rotation(const Matrix& mat) {
	for (Fast3d::Polygon& RotatePolygon : data) {
		for (Vector& RotateVector : RotatePolygon.data) {
			RotateVector *= mat;
		}
	}
}