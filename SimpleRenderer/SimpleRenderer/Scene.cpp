#include "Scene.h"

#include <algorithm>

Scene::Scene()
{
}

size_t Scene::createEntity()
{
	// Reuse destroyed entityID memory
	auto freeMem = std::find(m_componentMasks.begin(), m_componentMasks.end(), COMPONENT_NONE);
	if (freeMem != m_componentMasks.end())
		return *freeMem;

	// Allocate memory for new entityID
	m_componentMasks.push_back(COMPONENT_NONE);
	m_transformComponents.push_back({});
	m_velocityComponents.push_back({});
	m_angualarVelocityComponent.push_back({});
	m_meshComponents.push_back({});
	m_materialComponents.push_back({});

	return m_componentMasks.size() - 1;
}

void Scene::destroyEntity(size_t entityID)
{
	m_componentMasks.at(entityID) = COMPONENT_NONE;
}

size_t& Scene::getComponentMask(size_t entityID)
{
	return m_componentMasks.at(entityID);
}

size_t Scene::getComponentMask(size_t entityID) const
{
	return m_componentMasks.at(entityID);
}

glm::mat4& Scene::getTransformComponent(size_t entityID)
{
	return m_transformComponents.at(entityID);
}

const glm::mat4 & Scene::getTransformComponent(size_t entityID) const
{
	return m_transformComponents.at(entityID);
}

glm::vec3 & Scene::getVelocityComponent(size_t entityID)
{
	return m_velocityComponents.at(entityID);
}

const glm::vec3 & Scene::getVelocityComponent(size_t entityID) const
{
	return m_velocityComponents.at(entityID);
}

glm::vec3 & Scene::getAngularVelocityComponent(size_t entityID)
{
	return m_angualarVelocityComponent.at(entityID);
}

const glm::vec3 & Scene::getAngularVelocityComponent(size_t entityID) const
{
	return m_angualarVelocityComponent.at(entityID);
}

MeshComponent& Scene::getMeshComponent(size_t entityID)
{
	return m_meshComponents.at(entityID);
}

const MeshComponent & Scene::getMeshComponent(size_t entityID) const
{
	return m_meshComponents.at(entityID);
}

MaterialComponent & Scene::getMaterialComponent(size_t entityID)
{
	return m_materialComponents.at(entityID);
}

const MaterialComponent & Scene::getMaterialComponent(size_t entityID) const
{
	return m_materialComponents.at(entityID);
}

size_t Scene::getEntityCount() const
{
	return m_componentMasks.size();
}
