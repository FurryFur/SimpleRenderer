#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

struct InputComponent {
	glm::vec3 axis;
	glm::vec3 orientationDelta;
	bool btn1Down;
	bool btn2Down;
	bool btn3Down;
	bool btn4Down;

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
	int btn1Map;
	int btn2Map;
	int btn3Map;
	int btn4Map;
};