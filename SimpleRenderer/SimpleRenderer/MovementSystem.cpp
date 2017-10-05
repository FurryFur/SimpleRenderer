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

	float deltaYaw = -lookSensitivity * input.lookDelta.x;
	float deltaPitch = -lookSensitivity * input.lookDelta.y;
	glm::vec3 cameraFront = transform * glm::vec4{ 0, 0, -1, 0 }; // Convert to orientation space
	glm::vec3 cameraPos = transform[3];
	glm::vec3 cameraUp = glm::vec3{ 0, 1, 0 };
	
	// Displacement
	glm::vec3 moveAxis = GLMUtils::limitVec(input.moveAxis, 1);
	moveAxis = transform * glm::vec4{ moveAxis, 0 }; // Convert to orientation space
	cameraPos += moveSpeed * moveAxis;

	// Rotation
	cameraFront = glm::rotate(cameraFront, deltaPitch, glm::cross(cameraFront, cameraUp));
	cameraFront = glm::rotate(cameraFront, deltaYaw, cameraUp);

	transform = glm::inverse(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
}
