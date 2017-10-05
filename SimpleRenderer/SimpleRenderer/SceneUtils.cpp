#include "SceneUtils.h"

#include "GLUtils.h"
#include "Scene.h"

#include <glm\gtc\matrix_transform.hpp>

size_t SceneUtils::createEntity(Scene& scene)
{
	// Reuse destroyed entityID memory
	auto freeMem = std::find(scene.componentMasks.begin(), scene.componentMasks.end(), COMPONENT_NONE);
	if (freeMem != scene.componentMasks.end())
		return *freeMem;

	// Allocate memory for new entityID
	scene.componentMasks.emplace_back(COMPONENT_NONE);
	scene.transformComponents.emplace_back();
	scene.velocityComponents.emplace_back();
	scene.angualarVelocityComponent.emplace_back();
	scene.meshComponents.emplace_back();
	scene.materialComponents.emplace_back();
	scene.inputComponents.emplace_back();
	scene.movementComponents.emplace_back();

	return scene.componentMasks.size() - 1;
}

void SceneUtils::destroyEntity(Scene& scene, size_t entityID)
{
	scene.componentMasks.at(entityID) = COMPONENT_NONE;
}

size_t SceneUtils::getEntityCount(const Scene& scene)
{
	return scene.componentMasks.size();
}


size_t SceneUtils::createQuad(Scene& scene, const glm::mat4& transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;

	// Get references to components
	scene.transformComponents.at(entityID) = transform;
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/PlaneTexture.jpg");

	const std::vector<VertexFormat>& vertices = getQuadVertices();
	const std::vector<GLuint>& indices = getQuadIndices();
	mesh.VAO = GLUtils::bufferVertices(vertices, indices);
	mesh.numIndices = static_cast<GLsizei>(indices.size());

	return entityID;
}

size_t SceneUtils::createCamera(Scene& scene, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
{
	size_t entityID = createEntity(scene);

	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask = COMPONENT_CAMERA | COMPONENT_INPUT | COMPONENT_MOVEMENT | COMPONENT_TRANSFORM;

	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	glm::mat4& transform = scene.transformComponents.at(entityID);

	movementVars.moveSpeed = 0.1f;
	movementVars.lookSensitivity = 0.005f;

	transform = glm::inverse(glm::lookAt(pos, center, glm::vec3{ 0, 1, 0 }));

	return entityID;
}

const std::vector<VertexFormat>& SceneUtils::getQuadVertices()
{
	static const std::vector<VertexFormat> vertices = {
		{ { -1,  1, 0 },{ -1, 1, 1 },{ 0, 1 } }, // Top left
		{ { 1,  1, 0 },{ 1, 1, 1 },{ 1, 1 } }, // Top right
		{ { 1, -1, 0 },{ 1, -1, 1 },{ 1, 0 } }, // Bottom right
		{ { -1, -1, 0 },{ -1, -1, 1 },{ 0, 0 } }  // Bottom left
	};

	return vertices;
}

const std::vector<GLuint>& SceneUtils::getQuadIndices()
{
	static const std::vector<GLuint> indices = {
		0, 1, 2,
		0, 2, 3
	};

	return indices;
}

