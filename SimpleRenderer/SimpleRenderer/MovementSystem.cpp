#include "MovementSystem.h"

#include "MovementComponent.h"
#include "GLUtils.h"
#include "GLMUtils.h"
#include "Scene.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>

MovementSystem::MovementSystem(Scene & scene)
	: m_scene{ scene }
{
}

void MovementSystem::update(size_t entityID)
{
	// Filter movable
	const size_t kMovableMask = COMPONENT_MOVEMENT | COMPONENT_INPUT | COMPONENT_TRANSFORM;
	if ((m_scene.componentMasks.at(entityID) & kMovableMask) != kMovableMask)
		return;

	glm::mat4& transform = m_scene.transformComponents.at(entityID);
	MovementComponent& movementVars = m_scene.movementComponents.at(entityID);
	InputComponent& input = m_scene.inputComponents.at(entityID);

	float moveSpeed = movementVars.moveSpeed;
	float lookSensitivity = movementVars.lookSensitivity;

	float deltaYaw = -lookSensitivity * input.mouseDelta.x;
	float deltaPitch = -lookSensitivity * input.mouseDelta.y;
	glm::vec3 front = transform * glm::vec4{ 0, 0, -1, 0 }; // Convert to orientation space
	glm::vec3 pos = transform[3];
	glm::vec3 up = glm::vec3{ 0, 1, 0 };
	
	// Displacement
	glm::vec3 axis = GLMUtils::limitVec(input.axis, 1);
	axis = transform * glm::vec4{ axis, 0 }; // Convert to orientation space
	pos += moveSpeed * axis;

	// Rotation
	front = glm::rotate(front, deltaPitch, glm::cross(front, up));
	front = glm::rotate(front, deltaYaw, up);

	transform = glm::inverse(glm::lookAt(pos, pos + front, up));
}
