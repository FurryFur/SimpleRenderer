#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>
#include <string>

class Vertex;
class GLFWwindow;
class Scene;

namespace GLUtils {
	GLFWwindow* InitOpenGL();
	//GLuint loadTexture(const std::string& filename);

	size_t createQuad(Scene&, glm::mat4 transform);
	//size_t createCylinder(Scene&, glm::mat4 transform, size_t radius, size_t length);

	std::vector<Vertex>& getQuadVertices();
	std::vector<GLuint>& getQuadIndices();
	GLuint getDefaultShader();

	void bufferVertices(GLuint VAO, std::vector<Vertex> vertices, std::vector<GLuint> indices);
}