#include "Object.h"
using namespace Console3D;

Object::Object() : data() {};
Object::Object(const std::vector<Console3D::Polygon> data) : data(data) {};

void Object::Rotation(const Matrix& mat) {
	for (Console3D::Polygon& RotatePolygon : data) {
		for (Vector& RotateVector : RotatePolygon.data) {
			RotateVector *= mat;
		}
	}
}