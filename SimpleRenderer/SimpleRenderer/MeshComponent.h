#pragma once

#include "VertexFormat.h"

#include <glad\glad.h>

#include <vector>

struct MeshComponent {
	GLuint VAO;
	GLsizei numIndices;
};