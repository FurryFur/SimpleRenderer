#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

struct InputComponent {
	glm::vec3 axis;
	glm::vec3 orientationDelta;

	int leftBtnMap;
	int rightBtnMap;
	int forwardBtnMap;
	int backwardBtnMap;
	int upBtnMap;
	int downBtnMap;
	int azimuthPosBtnMap;
	int azimuthNegBtnMap;
	int elevationPosBtnMap;
	int elevationNegBtnMap;
	int rollBtnMap;
	bool mouseInputEnabled;
};