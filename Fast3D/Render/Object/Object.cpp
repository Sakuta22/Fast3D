#include "Object.h"
using namespace Console3D;

Object::Object() : data(), colorStroke(L'*'), colorFill(L'@') {};
Object::Object(const std::vector<Console3D::Polygon> data) : data(data), colorStroke(L'*'), colorFill(L'@') {}
Console3D::Object::Object(const std::vector<Console3D::Polygon> data, wchar_t colorCommon) : data(data), colorStroke(colorCommon), colorFill(colorCommon) {}
Console3D::Object::Object(const std::vector<Console3D::Polygon> data, wchar_t colorStroke, wchar_t colorFill) : data(data), colorStroke(colorStroke), colorFill(colorFill) {}

void Object::Rotation(const Matrix& mat) {
	for (Console3D::Polygon& RotatePolygon : data) {
		for (Vector& RotateVector : RotatePolygon.data) {
			RotateVector *= mat;
		}
	}
}