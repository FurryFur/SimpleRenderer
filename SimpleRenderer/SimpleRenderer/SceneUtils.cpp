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
	scene.logicComponents.emplace_back();

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
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_LOGIC;

	// Get references to components
	scene.transformComponents.at(entityID) = transform;
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	LogicComponent& logicVars = scene.logicComponents.at(entityID);

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/random-texture3.png");
	material.textureType = GL_TEXTURE_2D;
	material.enableDepth = true;
	material.shaderParams.metallicness = 1.0f;
	material.shaderParams.glossiness = 75.0f; // TODO: Fix values getting messed up on the gpu when this is 0 for some reason

	setDefaultInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	mesh = getQuadMesh();

	logicVars.rotationAxis = glm::vec3{ 0, 0, 1 };

	return entityID;
}

size_t SceneUtils::createSphere(Scene& scene, const glm::mat4& _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_LOGIC;

	// Get references to components
	glm::mat4& transform = scene.transformComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	LogicComponent& logicVars = scene.logicComponents.at(entityID);

	transform = _transform;

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/random-texture2.jpg");
	material.textureType = GL_TEXTURE_2D;
	material.enableDepth = true;
	material.shaderParams.metallicness = 0.3f;
	material.shaderParams.glossiness = 2.0f; // TODO: Fix values getting messed up on the gpu when this is 0 for some reason

	mesh = getSphereMesh();

	setDefaultInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	logicVars.rotationAxis = glm::vec3{ 0, 1, 0 };

	return entityID;
}

size_t SceneUtils::createCylinder(Scene& scene, float radius, float height, const glm::mat4& _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_LOGIC;

	// Get references to components
	glm::mat4& transform = scene.transformComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	LogicComponent& logicVars = scene.logicComponents.at(entityID);

	transform = _transform * glm::scale(glm::mat4{ 1 }, glm::vec3{ radius, height, radius });

	material.shader = GLUtils::getThresholdShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/random-texture4.jpg");
	material.textureType = GL_TEXTURE_2D;
	material.enableDepth = true;
	material.shaderParams.metallicness = 0.75f;
	material.shaderParams.glossiness = 40.0f; // TODO: Fix values getting messed up on the gpu when this is 0 for some reason

	mesh = getCylinderMesh();

	setDefaultInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	logicVars.rotationAxis = glm::vec3{ 0, 1, 0 };

	return entityID;
}

size_t SceneUtils::createPyramid(Scene& scene, const glm::mat4& _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_LOGIC;

	// Get references to components
	glm::mat4& transform = scene.transformComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	LogicComponent& logicVars = scene.logicComponents.at(entityID);

	transform = _transform;

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/random-texture.jpg");
	material.textureType = GL_TEXTURE_2D;
	material.enableDepth = true;
	material.shaderParams.metallicness = 0.95f;
	material.shaderParams.glossiness = 10.0f; // TODO: Fix values getting messed up on the gpu when this is 0 for some reason

	mesh = getPyramidMesh();

	setDefaultInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	logicVars.rotationAxis = glm::vec3{ 0, 1, 0 };

	return entityID;
}

size_t SceneUtils::createCube(Scene& scene, const glm::mat4 & _transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM | COMPONENT_LOGIC;

	// Get references to components
	glm::mat4& transform = scene.transformComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	LogicComponent& logicVars = scene.logicComponents.at(entityID);

	transform = _transform;

	material.shader = GLUtils::getDefaultShader();
	material.texture = GLUtils::loadTexture("Assets/Textures/random-texture3.png");
	material.textureType = GL_TEXTURE_2D;
	material.enableDepth = true;
	material.shaderParams.metallicness = 0.95f;
	material.shaderParams.glossiness = 10.0f; // TODO: Fix values getting messed up on the gpu when this is 0 for some reason

	mesh = getCubeMesh();

	setDefaultInputBindings(input);

	movementVars.moveSpeed = 0.1f;
	movementVars.orientationSensitivity = 0.05f;
	movementVars.worldSpaceMove = true;

	logicVars.rotationAxis = glm::vec3{ 0, 1, 0 };

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

size_t SceneUtils::createSkybox(Scene& scene, const std::vector<std::string>& faceFilenames)
{
	size_t entityID = createEntity(scene);

	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask = COMPONENT_MATERIAL | COMPONENT_MESH;

	MaterialComponent& material = scene.materialComponents.at(entityID);
	InputComponent& input = scene.inputComponents.at(entityID);
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	MeshComponent& mesh = scene.meshComponents.at(entityID);

	material = {};
	material.shader = GLUtils::getSkyboxShader();
	material.texture = GLUtils::loadCubeMap(faceFilenames);
	material.textureType = GL_TEXTURE_CUBE_MAP;
	material.enableDepth = false;

	mesh = getCubeMesh();

	return entityID;
}

void SceneUtils::setDefaultInputBindings(InputComponent& input)
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
	input.btn1Map = GLFW_KEY_KP_ADD;
	input.btn2Map = GLFW_KEY_KP_SUBTRACT;
	input.btn3Map = GLFW_KEY_KP_MULTIPLY;
	input.btn4Map = GLFW_KEY_KP_DIVIDE;
}

const std::vector<VertexFormat>& SceneUtils::getSphereVertices()
{
	static std::vector<VertexFormat> s_vertices;

	if (s_vertices.size() == 0) {
		s_vertices.reserve((g_kSphereThetaSegments + 1) * (g_kSpherePhiSegments + 1));
		// Theta is zenith angle from top of sphere
		for (size_t i = 0; i < g_kSphereThetaSegments + 1; ++i) {
			for (size_t j = 0; j < g_kSpherePhiSegments + 1; ++j) { // Output extra vertices for seem texture coordinates
				float theta = i * g_kDThetaSphere;
				float phi = j * g_kDPhiSphere;
				glm::vec3 position = { -sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi) };
				s_vertices.emplace_back(VertexFormat{
					position,                    // Position
					position,                    // Normal (same as position for a unit sphere)
					glm::vec2{ phi, -theta } }); // Texture Coordinate
			}
		}
	}

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getSphereIndices()
{
	static std::vector<GLuint> s_indices;

	if (s_indices.size() == 0) {
		s_indices.reserve(g_kSphereThetaSegments * g_kSpherePhiSegments * 6);
		for (GLuint i = 0; i < g_kSphereThetaSegments; ++i) {
			for (GLuint j = 0; j < g_kSpherePhiSegments; ++j) {
				GLuint topLeft = i * (g_kSpherePhiSegments + 1) + j;          // Need to add 1 for extra texCoord vertices at seem
				GLuint topRight = topLeft + 1;
				GLuint bottomLeft = (i + 1) * (g_kSpherePhiSegments + 1) + j; // Need to add 1 for extra texCoord vertices at seem
				GLuint bottomRight = bottomLeft + 1;

				s_indices.push_back(topLeft);
				s_indices.push_back(bottomLeft);
				s_indices.push_back(bottomRight);
				s_indices.push_back(topLeft);
				s_indices.push_back(bottomRight);
				s_indices.push_back(topRight);
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
		s_vertices.reserve(2 * (g_kCylinderThetaSegments + 1));
		// There are two groups of vertices in a cylinder
		// One group at the top, and one at the bottom.
		for (size_t i = 0; i < 2; ++i) {
			for (size_t j = 0; j < g_kCylinderThetaSegments + 1; ++j) {  // Output extra vertices for seem texture coordinates
				float theta = j * g_kDThetaCylinder;
				float y = height / 2 - i * height;
				glm::vec3 position = { cos(theta), y, -sin(theta) };
				s_vertices.emplace_back(VertexFormat{
					position,                                             // Position
					glm::normalize(glm::vec3(position.x, 0, position.z)), // Normal
					glm::vec2{ theta * 2 / M_PI, (1 - i) } });            // Texture Coordinate
			}
		}
	}

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getCylinderIndices()
{
	static std::vector<GLuint> s_indices;

	if (s_indices.size() == 0) {
		s_indices.reserve(g_kCylinderThetaSegments * 6);
		for (size_t i = 0; i < g_kCylinderThetaSegments; ++i) {
			GLuint topLeft = static_cast<GLuint>(i);
			GLuint topRight = topLeft + 1;
			GLuint bottomLeft = static_cast<GLuint>(g_kCylinderThetaSegments + 1 + i);
			GLuint bottomRight = bottomLeft + 1;

			s_indices.push_back(topLeft);
			s_indices.push_back(bottomLeft);
			s_indices.push_back(bottomRight);
			s_indices.push_back(topLeft);
			s_indices.push_back(bottomRight);
			s_indices.push_back(topRight);
		}
	}

	return s_indices;
}

const std::vector<VertexFormat>& SceneUtils::getPyramidVertices()
{
	static const glm::vec3 top =        { -0,  1,  0 };
	static const glm::vec3 frontLeft =  { -1, -1,  1 };
	static const glm::vec3 frontRight = {  1, -1,  1 };
	static const glm::vec3 backLeft =   { -1, -1, -1 };
	static const glm::vec3 backRight =  {  1, -1, -1 };
	static const glm::vec3 frontNormal  = glm::normalize(glm::cross(frontLeft - top, frontRight - frontLeft));
	static const glm::vec3 leftNormal   = glm::normalize(glm::cross(backLeft - top,  frontLeft - backLeft));
	static const glm::vec3 rightNormal  = glm::normalize(glm::cross(frontRight - top, backRight - frontRight));
	static const glm::vec3 backNormal   = glm::normalize(glm::cross(backRight - top, backLeft - backRight));
	static const glm::vec3 bottomNormal = glm::normalize(glm::cross(frontLeft - frontRight, backLeft - frontLeft));
	static const std::vector<VertexFormat> s_vertices = {
		// Front side
		{ top,        frontNormal,{ 0.5, 1 } },
		{ frontLeft,  frontNormal,{ 0,   0 } },
		{ frontRight, frontNormal,{ 1,   0 } },

		// Back side
		{ top,       backNormal,{ 0.5, 1 } },
		{ backRight, backNormal,{ 0,   0 } },
		{ backLeft,  backNormal,{ 1,   0 } },

		// Left side
		{ top,       leftNormal,{ 0.5, 1 } },
		{ backLeft,  leftNormal,{ 0,   0 } },
		{ frontLeft, leftNormal,{ 1,   0 } },

		// Right side
		{ top,        rightNormal,{ 0.5, 1 } },
		{ frontRight, rightNormal,{ 0,   0 } },
		{ backRight,  rightNormal,{ 1,   0 } },

		// Bottom
		{ frontRight, bottomNormal, { 1, 1 } },
		{ frontLeft,  bottomNormal, { 0, 1 } },
		{ backLeft,   bottomNormal, { 0, 0 } },
		{ backRight,  bottomNormal, { 1, 0 } },
	};

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getPyramidIndices()
{
	static const std::vector<GLuint> s_indices = {
		// Front side
		0, 1, 2,

		// Back side
		3, 4, 5,

		// Left side
		6, 7, 8,

		// Right side
		9, 10, 11,

		// Bottom
		12, 13, 14,
		12, 14, 15
	};

	return s_indices;
}

const std::vector<VertexFormat>& SceneUtils::getCubeVertices()
{
	static const glm::vec3 topFrontLeft =     { -1,  1,  1 };
	static const glm::vec3 topFrontRight =    {  1,  1,  1 };
	static const glm::vec3 topBackLeft =      { -1,  1, -1 };
	static const glm::vec3 topBackRight =     {  1,  1, -1 };
	static const glm::vec3 bottomFrontLeft =  { -1, -1,  1 };
	static const glm::vec3 bottomFrontRight = {  1, -1,  1 };
	static const glm::vec3 bottomBackLeft =   { -1, -1, -1 };
	static const glm::vec3 bottomBackRight =  {  1, -1, -1 };
	static const glm::vec3 topNormal =    {  0,  1,  0 };
	static const glm::vec3 frontNormal =  {  0,  0,  1 };
	static const glm::vec3 leftNormal =   { -1,  0,  0 };
	static const glm::vec3 rightNormal =  {  1,  0,  0 };
	static const glm::vec3 backNormal =   {  0,  0, -1 };
	static const glm::vec3 bottomNormal = {  0, -1,  0 };
	static const std::vector<VertexFormat> s_vertices = {
		// Top side
		{ topBackRight,  topNormal, { 1, 1 } },
		{ topBackLeft,   topNormal, { 0, 1 } },
		{ topFrontLeft,  topNormal, { 0, 0 } },
		{ topFrontRight, topNormal, { 1, 0 } },

		// Front side
		{ topFrontRight,    frontNormal, { 1, 1 } },
		{ topFrontLeft,     frontNormal, { 0, 1 } },
		{ bottomFrontLeft,  frontNormal, { 0, 0 } },
		{ bottomFrontRight, frontNormal, { 1, 0 } },

		// Back side
		{ topBackLeft,     backNormal, { 1, 1 } },
		{ topBackRight,    backNormal, { 0, 1 } },
		{ bottomBackRight, backNormal, { 0, 0 } },
		{ bottomBackLeft,  backNormal, { 1, 0 } },

		// Left side
		{ topFrontLeft,    leftNormal, { 1, 1 } },
		{ topBackLeft,     leftNormal, { 0, 1 } },
		{ bottomBackLeft,  leftNormal, { 0, 0 } },
		{ bottomFrontLeft, leftNormal, { 1, 0 } },

		// Right side
		{ topBackRight,     rightNormal, { 0, 1 } },
		{ topFrontRight,    rightNormal, { 1, 1 } },
		{ bottomFrontRight, rightNormal, { 1, 0 } },
		{ bottomBackRight,  rightNormal, { 0, 0 } },

		// Bottom
		{ bottomFrontRight, bottomNormal, { 1, 1 } },
		{ bottomFrontLeft,  bottomNormal, { 0, 1 } },
		{ bottomBackLeft,   bottomNormal, { 0, 0 } },
		{ bottomBackRight,  bottomNormal, { 1, 0 } },
	};

	return s_vertices;
}

const std::vector<GLuint>& SceneUtils::getCubeIndices()
{
	static const std::vector<GLuint> s_indices = {
		// Top side
		0, 1, 2,
		0, 2, 3,

		// Front side
		4, 5, 6,
		4, 6, 7,

		// Back side
		8, 9, 10,
		8, 10, 11,

		// Left side
		12, 13, 14,
		12, 14, 15,

		// Right side
		16, 17, 18,
		16, 18, 19,

		// Bottom side
		20, 21, 22,
		20, 22, 23
	};

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

MeshComponent SceneUtils::getPyramidMesh()
{
	static const std::vector<VertexFormat>& vertices = getPyramidVertices();
	static const std::vector<GLuint>& indices = getPyramidIndices();
	static const MeshComponent mesh{
		GLUtils::bufferVertices(vertices, indices),
		static_cast<GLsizei>(indices.size())
	};

	return mesh;
}

MeshComponent SceneUtils::getCubeMesh()
{
	static const std::vector<VertexFormat>& vertices = getCubeVertices();
	static const std::vector<GLuint>& indices = getCubeIndices();
	static const MeshComponent mesh{
		GLUtils::bufferVertices(vertices, indices),
		static_cast<GLsizei>(indices.size())
	};

	return mesh;
}

