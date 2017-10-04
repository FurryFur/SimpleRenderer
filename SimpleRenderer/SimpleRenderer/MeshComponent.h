#pragma once

#include "Vertex.h"

#include <glad\glad.h>

#include <vector>

struct MeshComponent {
	GLuint VAO;
	size_t numIndices;
};