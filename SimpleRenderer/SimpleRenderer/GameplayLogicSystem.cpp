#include "GameplayLogicSystem.h"

#include "InputSystem.h"
#include "InputComponent.h"
#include "ShaderParams.h"
#include "Scene.h"
#include "Utils.h"

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
	// Update the currently possessed entity
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

	const size_t kModifiableGlossMask = COMPONENT_MATERIAL | COMPONENT_INPUT;
	if ((m_scene.componentMasks.at(entityID) & kModifiableGlossMask) == kModifiableGlossMask) {
		// Alter the glossiness of the possessed entity on key event
		InputComponent& input = m_scene.inputComponents.at(entityID);
		ShaderParams& shaderParams = m_scene.materialComponents.at(entityID).shaderParams;
		if (input.btn1Down) {
			shaderParams.glossiness = clamp(shaderParams.glossiness + 0.1f, 0.001f, 100.0f);
		}
		if (input.btn2Down) {
			shaderParams.glossiness = clamp(shaderParams.glossiness - 0.1f, 0.001f, 100.0f);
		}
		if (input.btn3Down) {
			shaderParams.metallicness = clamp(shaderParams.metallicness + 0.01f, 0.0f, 1.0f);
		}
		if (input.btn4Down) {
			shaderParams.metallicness = clamp(shaderParams.metallicness - 0.01f, 0.0f, 1.0f);
		}
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
