#pragma once
#include <vector>
#include "../Matrix/Matrix.h"
#include "../Polygon/Polygon.h"

namespace Console3D {

struct Object {
	std::vector<Console3D::Polygon> data;
	wchar_t colorStroke;
	wchar_t colorFill;

	Object();
	Object(const std::vector<Console3D::Polygon> data);
	Object(const std::vector<Console3D::Polygon> data, wchar_t colorCommon);
	Object(const std::vector<Console3D::Polygon> data, wchar_t colorStroke, wchar_t colorFill);

	void Rotation(const Matrix& mat);
};

}