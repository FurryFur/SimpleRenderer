#pragma once

#include <glm\glm.hpp>

struct UniformFormat {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 cameraPos;
};