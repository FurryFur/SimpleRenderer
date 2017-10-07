#include "GameplayLogicSystem.h"

#include "InputSystem.h"
#include "Scene.h"

#include <GLFW\glfw3.h>

GameplayLogicSystem::GameplayLogicSystem(Scene& scene, InputSystem& inputSystem)
	: m_scene{ scene }
	, m_oldPossessedEntity{ 0 }
	, m_possessedEntity{ 0 }
	, m_oldPossessedEntityUpdated{ true }
	, m_newPossessedEntityUpdated{ false }
{
	inputSystem.registerKeyObserver(this);
}

void GameplayLogicSystem::update(size_t entityID)
{
	if (!m_oldPossessedEntityUpdated && entityID == m_oldPossessedEntity) {
		// Update the old possessed entity so it doesn't respond to input
		size_t& componentMask = m_scene.componentMasks.at(entityID);
		componentMask &= ~(COMPONENT_INPUT | COMPONENT_MOVEMENT);
		m_oldPossessedEntityUpdated = true;
		return;
	}

	if (!m_newPossessedEntityUpdated && entityID == m_possessedEntity) {
		// Update the new possessed entity so it response to input
		size_t& componentMask = m_scene.componentMasks.at(entityID);
		componentMask |= (COMPONENT_INPUT | COMPONENT_MOVEMENT);
		m_newPossessedEntityUpdated = true;
		return;
	}
}

void GameplayLogicSystem::keyCallback(int key, int scancode, int action, int mods)
{
	// Skip keys we aren't interested in
	if (!(key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3 || key == GLFW_KEY_4))
		return;

	// Update the possessed entity state (which entity is currently being controled)
	m_oldPossessedEntity = m_possessedEntity;

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		m_possessedEntity = 0;
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		m_possessedEntity = 1;
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		m_possessedEntity = 2;
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		m_possessedEntity = 3;

	m_oldPossessedEntityUpdated = false;
	m_newPossessedEntityUpdated = false;
}
