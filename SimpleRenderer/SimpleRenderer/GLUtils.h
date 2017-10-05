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

	size_t createQuad(Scene&, const glm::mat4& transform);
	//size_t createCylinder(Scene&, glm::mat4 transform, size_t radius, size_t length);
	size_t createCamera(Scene&, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up);

	const std::vector<VertexFormat>& getQuadVertices();
	const std::vector<GLuint>& getQuadIndices();
	GLuint getDefaultShader();

	GLuint bufferVertices(const std::vector<VertexFormat>& vertices, const std::vector<GLuint>& indices);
}