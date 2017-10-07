#define _USE_MATH_DEFINES

#include "MovementSystem.h"

#include "MovementComponent.h"
#include "GLUtils.h"
#include "GLMUtils.h"
#include "Scene.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>

#include <cmath>

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

	float deltaAzimuth = -lookSensitivity * input.orientationDelta.x;
	float deltaElevation = -lookSensitivity * input.orientationDelta.y;
	float roll = -lookSensitivity * input.orientationDelta.z;
	glm::vec3 front = glm::vec3{ 0, 0, -1 };
	if (!movementVars.worldSpaceMove)
		front = transform * glm::vec4{ front, 0 }; // Convert movement to local coordinates
	glm::vec3 pos = transform[3];
	glm::vec3 up = glm::vec3{ 0, 1, 0 };
	glm::vec3 right = glm::cross(front, up);
	
	// Displacement
	glm::vec3 axis = GLMUtils::limitVec(input.axis, 1);
	if (!movementVars.worldSpaceMove)
		axis = transform * glm::vec4{ axis, 0 }; // Convert movement to local coordinates
	pos += moveSpeed * axis;

	// Rotation
	// Prevent elevation going past 90 degrees
	glm::mat3 elevationMat{ 1 };
	float elevation = M_PI / 2 - std::acos(glm::dot(front, up));
	if (std::abs(elevation + deltaElevation) < M_PI / 2)
		elevationMat = glm::rotate(deltaElevation, right);
	glm::mat3 azimuthMat = glm::rotate(deltaAzimuth, up);
	glm::mat3 rollMat = glm::rotate(roll, front);

	transform[3] = {0, 0, 0, 1}; // Remove displacement temporarily
	transform = glm::mat4{ rollMat * azimuthMat * elevationMat } * transform; // Rotation without displacement
	transform = glm::translate(pos) * transform; // Add displacement back in
}
