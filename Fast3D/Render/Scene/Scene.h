#pragma once
#include <vector>
#include "../Object/Object.h"

namespace Console3D {

struct Scene {
	std::vector<Object> data;

	Scene();
	Scene(std::vector<Object> data);
};

}