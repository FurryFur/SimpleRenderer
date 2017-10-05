#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>
#include <string>

struct VertexFormat;
struct GLFWwindow;
struct Scene;
class InputSystem;

namespace GLUtils {
	GLFWwindow* initOpenGL();

	GLuint getDefaultShader();

	GLuint bufferVertices(const std::vector<VertexFormat>& vertices, const std::vector<GLuint>& indices);

	GLuint loadTexture(const std::string& filename);
}