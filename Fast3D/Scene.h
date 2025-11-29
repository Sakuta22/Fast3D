#pragma once
#include <vector>
#include "Object.h"

struct Scene {
	std::vector<Object> data;

	Scene();
	Scene(std::vector<Object> data);
};