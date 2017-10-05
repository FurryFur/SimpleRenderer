#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>

struct Scene;
struct VertexFormat;

namespace SceneUtils {
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
}