#pragma once

#include <glm\glm.hpp>

struct CameraComponent {
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};