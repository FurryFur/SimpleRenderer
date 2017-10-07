#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

struct InputComponent {
	glm::vec3 axis;
	glm::vec2 mouseDelta;

	int leftBtnMap;
	int rightBtnMap;
	int forwardBtnMap;
	int backwardBtnMap;
	int upBtnMap;
	int downBtnMap;
};