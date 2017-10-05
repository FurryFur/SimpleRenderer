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
	//GLuint loadTexture(const std::string& filename);

	// Creates a new entity in the scene and returns its ID
	size_t createEntity(Scene& scene);

	// Destroys an entity in the scene
	void destroyEntity(Scene& scene, size_t entityID);

	// Returns the number of entities in the scene
	size_t getEntityCount(const Scene& scene);

	size_t createQuad(Scene&, const glm::mat4& transform);

	//size_t createCylinder(Scene&, glm::mat4 transform, size_t radius, size_t length);

	size_t createCamera(Scene&, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up);

	const std::vector<VertexFormat>& getQuadVertices();
	const std::vector<GLuint>& getQuadIndices();

	GLuint getDefaultShader();

	GLuint bufferVertices(const std::vector<VertexFormat>& vertices, const std::vector<GLuint>& indices);

	// Limits a vector to the specified magnitude
	template <typename VecT>
	VecT limitVec(const VecT& vec, float maxMagnitude)
	{
		auto magnitude = glm::length(vec);
		if (magnitude > maxMagnitude) {
			return glm::normalize(vec) * maxMagnitude;
		}
		return vec;
	}
}