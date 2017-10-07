#define _USE_MATH_DEFINES

#include "SceneUtils.h"

#include "GLUtils.h"
#include "Scene.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include <cmath>

const size_t g_kThetaSegments = 16; // Number of segments from top to bottom of sphere
const size_t g_kPhiSegments = 16; // Number of segments going around the sphere
const float g_kDThetaSphere = static_cast<float>(M_PI / g_kThetaSegments);
const float g_kDPhiSphere = static_cast<float>(2 * M_PI / g_kPhiSegments);

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

	mesh = getQuadMesh();

	return entityID;
}

size_t SceneUtils::createSphere(Scene& scene, const glm::mat4& _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_INPUT | COMPONENT_MOVEMENT;

	// Get references to components
	glm::mat4& transform = scene.transformComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);

	transform = _transform;

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/PlaneTexture.jpg");

	mesh = getSphereMesh();

	input = {};
	input.leftBtnMap = GLFW_KEY_KP_4;
	input.rightBtnMap = GLFW_KEY_KP_6;
	input.forwardBtnMap = GLFW_KEY_KP_8;
	input.backwardBtnMap = GLFW_KEY_KP_5;
	input.downBtnMap = GLFW_KEY_KP_7;
	input.upBtnMap = GLFW_KEY_KP_9;
	input.azimuthPosBtnMap = GLFW_KEY_KP_1;
	input.azimuthNegBtnMap = GLFW_KEY_KP_2;
	input.elevationPosBtnMap = GLFW_KEY_KP_3;
	input.elevationNegBtnMap = GLFW_KEY_KP_DECIMAL;
	input.rollBtnMap = GLFW_KEY_KP_0;

	movementVars = {};
	movementVars.moveSpeed = 0.1f;
	movementVars.lookSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	return entityID;
}

size_t SceneUtils::createCamera(Scene& scene, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
{
	size_t entityID = createEntity(scene);

	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask = COMPONENT_CAMERA | COMPONENT_INPUT | COMPONENT_MOVEMENT | COMPONENT_TRANSFORM;

	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	glm::mat4& transform = scene.transformComponents.at(entityID);

	input = {};
	input.mouseInputEnabled = true;
	input.leftBtnMap = GLFW_KEY_A;
	input.rightBtnMap = GLFW_KEY_D;
	input.forwardBtnMap = GLFW_KEY_W;
	input.backwardBtnMap = GLFW_KEY_S;
	input.downBtnMap = GLFW_KEY_Q;
	input.upBtnMap = GLFW_KEY_E;

	movementVars.moveSpeed = 0.1f;
	movementVars.lookSensitivity = 0.005f;
	movementVars.worldSpaceMove = false;

	transform = glm::inverse(glm::lookAt(pos, center, glm::vec3{ 0, 1, 0 }));

	return entityID;
}

const std::vector<VertexFormat>& SceneUtils::getSphereVertices()
{
	static std::vector<VertexFormat> s_vertices;

	if (s_vertices.size() == 0) {
		// Theta is angle from top of sphere
		for (size_t i = 0; i < g_kThetaSegments + 1; ++i) {
			float theta = i * g_kDThetaSphere;
			for (size_t j = 0; j < g_kPhiSegments + 1; ++j) {
				float phi = j * g_kDPhiSphere;
				glm::vec3 position = { -sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi) };
				s_vertices.emplace_back(VertexFormat{
					position,                   // Position
					position,                   // Normal (same as position for a unit sphere)
					glm::vec2{ phi, -theta } }); // Texture Coordinate
			}
		}
	}

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getSphereIndices()
{
	static const size_t numVertices = (g_kThetaSegments + 1) * (g_kPhiSegments + 1);
	static std::vector<GLuint> s_indices;

	if (s_indices.size() == 0) {
		for (GLuint i = 0; i < g_kThetaSegments + 1; ++i) {
			for (GLuint j = 0; j < g_kPhiSegments + 1; ++j) {
				GLuint vertIdxTopLeft = i * g_kPhiSegments + j;
				GLuint vertIdxTopRight = vertIdxTopLeft + 1;
				GLuint vertIdxBottomLeft = (i + 1) * g_kPhiSegments + j;
				GLuint vertIdxBottomRight = vertIdxBottomLeft + 1;
				if ((vertIdxTopLeft < numVertices) && (vertIdxTopRight < numVertices)
					&& (vertIdxBottomLeft < numVertices) && (vertIdxBottomRight < numVertices)) {
					s_indices.push_back(vertIdxTopLeft);
					s_indices.push_back(vertIdxTopRight);
					s_indices.push_back(vertIdxBottomRight);
					s_indices.push_back(vertIdxTopLeft);
					s_indices.push_back(vertIdxBottomRight);
					s_indices.push_back(vertIdxBottomLeft);
				}
			}
		}
	}

	return s_indices;
}

const std::vector<VertexFormat>& SceneUtils::getQuadVertices()
{
	static const std::vector<VertexFormat> s_vertices = {
		{ { -1,  1, 0 },{ -1, 1, 1 },{ 0, 1 } }, // Top left
		{ { 1,  1, 0 },{ 1, 1, 1 },{ 1, 1 } }, // Top right
		{ { 1, -1, 0 },{ 1, -1, 1 },{ 1, 0 } }, // Bottom right
		{ { -1, -1, 0 },{ -1, -1, 1 },{ 0, 0 } }  // Bottom left
	};

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getQuadIndices()
{
	static const std::vector<GLuint> s_indices = {
		0, 1, 2,
		0, 2, 3
	};

	return s_indices;
}

MeshComponent SceneUtils::getQuadMesh()
{
	static const std::vector<VertexFormat>& vertices = getQuadVertices();
	static const std::vector<GLuint>& indices = getQuadIndices();
	static const MeshComponent mesh{
		GLUtils::bufferVertices(vertices, indices),
		static_cast<GLsizei>(indices.size())
	};

	return mesh;
}

MeshComponent SceneUtils::getSphereMesh()
{
	static const std::vector<VertexFormat>& vertices = getSphereVertices();
	static const std::vector<GLuint>& indices = getSphereIndices();
	static const MeshComponent mesh{
		GLUtils::bufferVertices(vertices, indices),
		static_cast<GLsizei>(indices.size())
	};

	return mesh;
}

