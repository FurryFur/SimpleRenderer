#define _USE_MATH_DEFINES

#include "SceneUtils.h"

#include "GLUtils.h"
#include "Scene.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include <cmath>

const size_t g_kSphereThetaSegments = 16; // Number of segments from top to bottom of sphere
const size_t g_kSpherePhiSegments = 16; // Number of segments going around the sphere
const size_t g_kCylinderThetaSegments = 16; // Number of segments going around the cylinder
const float g_kDThetaSphere = static_cast<float>(M_PI / g_kSphereThetaSegments);
const float g_kDPhiSphere = static_cast<float>(2 * M_PI / g_kSpherePhiSegments);
const float g_kDThetaCylinder = static_cast<float>(2 * M_PI / g_kCylinderThetaSegments);
const size_t g_kNumVerticesCylinder = (g_kCylinderThetaSegments + 1) * 2;
const size_t g_kNumVerticesSphere = (g_kSphereThetaSegments + 1) * (g_kSpherePhiSegments + 1);

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
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/PlaneTexture.jpg");

	setDefaultWorldMoveInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	mesh = getQuadMesh();

	return entityID;
}

size_t SceneUtils::createSphere(Scene& scene, const glm::mat4& _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;

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

	setDefaultWorldMoveInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	return entityID;
}

size_t SceneUtils::createCylinder(Scene& scene, float radius, float height, const glm::mat4& _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;

	// Get references to components
	glm::mat4& transform = scene.transformComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);

	transform = _transform;

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/PlaneTexture.jpg");

	mesh = getCylinderMesh();

	setDefaultWorldMoveInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
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
	movementVars.orientationSensitivity = 0.005f;
	movementVars.worldSpaceMove = false;

	transform = glm::inverse(glm::lookAt(pos, center, up));

	return entityID;
}

void SceneUtils::setDefaultWorldMoveInputBindings(InputComponent& input)
{
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
}

const std::vector<VertexFormat>& SceneUtils::getSphereVertices()
{
	static std::vector<VertexFormat> s_vertices;

	if (s_vertices.size() == 0) {
		s_vertices.reserve(g_kNumVerticesSphere);
		// Theta is angle from top of sphere
		for (size_t i = 0; i < g_kSphereThetaSegments + 1; ++i) {
			float theta = i * g_kDThetaSphere;
			for (size_t j = 0; j < g_kSpherePhiSegments + 1; ++j) {
				float phi = j * g_kDPhiSphere;
				glm::vec3 position = { -sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi) };
				s_vertices.emplace_back(VertexFormat{
					position,                   // Position
					position,                   // Normal (same as position for a unit sphere)
					glm::vec2{ phi, theta } }); // Texture Coordinate
			}
		}
	}

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getSphereIndices()
{
	static std::vector<GLuint> s_indices;

	if (s_indices.size() == 0) {
		s_indices.reserve(g_kNumVerticesSphere);
		for (GLuint i = 0; i < g_kSphereThetaSegments + 1; ++i) {
			for (GLuint j = 0; j < g_kSpherePhiSegments + 1; ++j) {
				GLuint vertIdxTopLeft = i * g_kSpherePhiSegments + j;
				GLuint vertIdxTopRight = vertIdxTopLeft + 1;
				GLuint vertIdxBottomLeft = (i + 1) * g_kSpherePhiSegments + j;
				GLuint vertIdxBottomRight = vertIdxBottomLeft + 1;
				if ((vertIdxTopLeft    < g_kNumVerticesSphere) && (vertIdxTopRight    < g_kNumVerticesSphere)
				&&  (vertIdxBottomLeft < g_kNumVerticesSphere) && (vertIdxBottomRight < g_kNumVerticesSphere)) {
					s_indices.push_back(vertIdxTopLeft);
					s_indices.push_back(vertIdxBottomRight);
					s_indices.push_back(vertIdxTopRight);
					s_indices.push_back(vertIdxTopLeft);
					s_indices.push_back(vertIdxBottomLeft);
					s_indices.push_back(vertIdxBottomRight);
				}
			}
		}
	}

	return s_indices;
}

const std::vector<VertexFormat>& SceneUtils::getCylinderVertices()
{
	static const float height = 1;
	static std::vector<VertexFormat> s_vertices;

	if (s_vertices.size() == 0) {
		s_vertices.reserve(g_kNumVerticesCylinder);
		// There are two groups of vertices in a cylinder
		// One group at the top, and one at the bottom.
		for (size_t i = 0; i < 2; ++i) {
			for (size_t j = 0; j < g_kCylinderThetaSegments + 1; ++j) {
				float theta = j * g_kDThetaCylinder;
				float y = height - i;
				glm::vec3 position = { cos(theta), y, -sin(theta) };
				s_vertices.emplace_back(VertexFormat{
					position,                                             // Position
					glm::normalize(glm::vec3(position.x, 0, position.z)), // Normal
					glm::vec2{ theta / M_2_PI, y / height } });           // Texture Coordinate
			}
		}
	}

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getCylinderIndices()
{
	static std::vector<GLuint> s_indices;

	if (s_indices.size() == 0) {
		s_indices.reserve(g_kNumVerticesCylinder);
		for (size_t i = 0; i < 2; ++i) {
			for (size_t j = 0; j < g_kCylinderThetaSegments + 1; ++j) {
				GLuint vertIdxTopLeft = i * g_kCylinderThetaSegments + j;
				GLuint vertIdxTopRight = vertIdxTopLeft + 1;
				GLuint vertIdxBottomLeft = (i + 1) * g_kCylinderThetaSegments + j;
				GLuint vertIdxBottomRight = vertIdxBottomLeft + 1;
				if ((vertIdxTopLeft   < g_kNumVerticesCylinder)  && (vertIdxTopRight    < g_kNumVerticesCylinder)
				&&  (vertIdxBottomLeft < g_kNumVerticesCylinder) && (vertIdxBottomRight < g_kNumVerticesCylinder)) {
					s_indices.push_back(vertIdxTopLeft);
					s_indices.push_back(vertIdxBottomRight);
					s_indices.push_back(vertIdxTopRight);
					s_indices.push_back(vertIdxTopLeft);
					s_indices.push_back(vertIdxBottomLeft);
					s_indices.push_back(vertIdxBottomRight);
				}
			}
		}
	}

	return s_indices;
}

const std::vector<VertexFormat>& SceneUtils::getQuadVertices()
{
	static const std::vector<VertexFormat> s_vertices = {
		{ { -1,  1, 0 }, { 0, 0, 1 }, { 0, 1 } }, // Top left
		{ {  1,  1, 0 }, { 0, 0, 1 }, { 1, 1 } }, // Top right
		{ {  1, -1, 0 }, { 0, 0, 1 }, { 1, 0 } }, // Bottom right
		{ { -1, -1, 0 }, { 0, 0, 1 }, { 0, 0 } }  // Bottom left
	};

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getQuadIndices()
{
	static const std::vector<GLuint> s_indices = {
		0, 2, 1,
		0, 3, 2
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

MeshComponent SceneUtils::getCylinderMesh()
{
	static const std::vector<VertexFormat>& vertices = getCylinderVertices();
	static const std::vector<GLuint>& indices = getCylinderIndices();
	static const MeshComponent mesh{
		GLUtils::bufferVertices(vertices, indices),
		static_cast<GLsizei>(indices.size())
	};

	return mesh;
}

