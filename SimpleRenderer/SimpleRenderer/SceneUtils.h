#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>

struct Scene;
struct VertexFormat;
struct MeshComponent;
struct InputComponent;

namespace SceneUtils {
	// Creates a new entity in the scene and returns its ID
	size_t createEntity(Scene& scene);

	// Destroys an entity in the scene
	void destroyEntity(Scene& scene, size_t entityID);

	// Returns the number of entities in the scene
	size_t getEntityCount(const Scene& scene);

	// Creates a unit square facing down the positive z axis with the 
	// specified transform
	size_t createQuad(Scene&, const glm::mat4& transform = glm::mat4{ 1 });

	// Creates a unit sphere, centered at the origin, with the specified 
	// transform;
	size_t createSphere(Scene&, const glm::mat4& transform = glm::mat4{ 1 });

	// Create a cylinder with the specified radius and height.
	// The center of the base will be at the origin by default,
	// Unless a transform is specified.
	size_t createCylinder(Scene&, float radius, float height, const glm::mat4& transform);

	size_t createPyramid(Scene&, const glm::mat4& transform);

	size_t createCamera(Scene&, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up = glm::vec3{ 0, 1, 0 });

	void setDefaultInputBindings(InputComponent& input);

	const std::vector<VertexFormat>& getQuadVertices();
	const std::vector<GLuint>& getQuadIndices();
	const std::vector<VertexFormat>& getSphereVertices();
	const std::vector<GLuint>& getSphereIndices();
	const std::vector<VertexFormat>& getCylinderVertices();
	const std::vector<GLuint>& getCylinderIndices();
	const std::vector<VertexFormat>& getPyramidVertices();
	const std::vector<GLuint>& getPyramidIndices();
	MeshComponent getQuadMesh();
	MeshComponent getSphereMesh();
	MeshComponent getCylinderMesh();
	MeshComponent getPyramidMesh();
}