#pragma once

#include "MeshComponent.h"
#include "MaterialComponent.h"

#include <glm\glm.hpp>

#include <vector>

enum ComponentMask {
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM = 1 << 0,
	COMPONENT_VELOCITY = 1 << 1,
	COMPONENT_ANGULAR_VELOCITY = 1 << 2,
	COMPONENT_MESH = 1 << 3,
	COMPONENT_MATERIAL = 1 << 4
};

class Scene {
public:
	Scene();

	// Creates a new entity in the scene and returns its ID
	size_t createEntity();

	// Destroys an entity
	void destroyEntity(size_t entityID);

	size_t& getComponentMask(size_t entityID);
	size_t getComponentMask(size_t entityID) const;
	glm::mat4& getTransformComponent(size_t entityID);
	const glm::mat4& getTransformComponent(size_t entityID) const;
	glm::vec3& getVelocityComponent(size_t entityID);
	const glm::vec3& getVelocityComponent(size_t entityID) const;
	glm::vec3& getAngularVelocityComponent(size_t entityID);
	const glm::vec3& getAngularVelocityComponent(size_t entityID) const;
	MeshComponent& getMeshComponent(size_t entityID);
	const MeshComponent& getMeshComponent(size_t entityID) const;
	MaterialComponent& getMaterialComponent(size_t entityID);
	const MaterialComponent& getMaterialComponent(size_t entityID) const;
	
	size_t getEntityCount() const;

private:
	std::vector<size_t> m_componentMasks;
	std::vector<glm::mat4> m_transformComponents;
	std::vector<glm::vec3> m_velocityComponents;
	std::vector<glm::vec3> m_angualarVelocityComponent;
	std::vector<MeshComponent> m_meshComponents;
	std::vector<MaterialComponent> m_materialComponents;
};