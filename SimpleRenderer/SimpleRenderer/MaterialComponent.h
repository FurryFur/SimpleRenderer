#pragma once

#include "ShaderParams.h"

#include <glad\glad.h>

struct MaterialComponent {
	GLuint shader;
	GLuint texture;
	ShaderParams shaderParams;
};