#pragma once
#include "scene.h"

using namespace ppkin;
scene::scene()
{
	for (float i = -5; i < 10; i += 1) {
		for (float y = -5; y < 10; y += 1) {
			trainglePositions.push_back(glm::vec3(i, y, 0.0f));
		}
}
	
}
