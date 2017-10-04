#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>
#include <string>

struct VertexFormat;
struct GLFWwindow;
class Scene;

namespace GLUtils {
	GLFWwindow* InitOpenGL();
	//GLuint loadTexture(const std::string& filename);

	size_t createQuad(Scene&, glm::mat4 transform);
	//size_t createCylinder(Scene&, glm::mat4 transform, size_t radius, size_t length);

	const std::vector<VertexFormat>& getQuadVertices();
	const std::vector<GLuint>& getQuadIndices();
	GLuint getDefaultShader();

	GLuint bufferVertices(const std::vector<VertexFormat>& vertices, const std::vector<GLuint>& indices);
}