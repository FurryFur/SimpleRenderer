#include "GLUtils.h"

#include "InputSystem.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "MovementComponent.h"
#include "Scene.h"
#include "ShaderHelper.h"
#include "VertexFormat.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

#define BUFFER_OFFSET(i) ((GLvoid *)(i*sizeof(float)))

// Callback for handling glfw errors
void errorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// Handles glContext resize events
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* GLUtils::initOpenGL()
{
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	// Create opengl glContext and glContext
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* glContext = glfwCreateWindow(800, 800, "Simple Renderer", nullptr, nullptr);
	if (!glContext)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(glContext);

	// Register callbacks
	//glfwSetKeyCallback(glContext, keyCallback);
	glfwSetFramebufferSizeCallback(glContext, framebufferSizeCallback);

	// Load opengl functinos
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Configure glContext
	glfwSwapInterval(1);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Setup opengl viewport
	int width, height;
	glfwGetFramebufferSize(glContext, &width, &height);
	glViewport(0, 0, width, height);

	return glContext;
}

size_t GLUtils::createEntity(Scene& scene)
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

void GLUtils::destroyEntity(Scene& scene, size_t entityID)
{
	scene.componentMasks.at(entityID) = COMPONENT_NONE;
}

size_t GLUtils::getEntityCount(const Scene& scene)
{
	return scene.componentMasks.size();
}


size_t GLUtils::createQuad(Scene& scene, const glm::mat4& transform)
{
	size_t entityID = createEntity(scene);
	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;

	// Get references to components
	scene.transformComponents.at(entityID) = transform;
	MeshComponent& mesh = scene.meshComponents.at(entityID);
	MaterialComponent& material = scene.materialComponents.at(entityID);

	material.shader = getDefaultShader();

	const std::vector<VertexFormat>& vertices = getQuadVertices();
	const std::vector<GLuint>& indices = getQuadIndices();
	mesh.VAO = bufferVertices(vertices, indices);
	mesh.numIndices = static_cast<GLsizei>(indices.size());

	return entityID;
}

size_t GLUtils::createCamera(Scene& scene, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
{
	size_t entityID = createEntity(scene);

	size_t& componentMask = scene.componentMasks.at(entityID);
	componentMask = COMPONENT_CAMERA | COMPONENT_INPUT | COMPONENT_MOVEMENT | COMPONENT_TRANSFORM;
	
	MovementComponent& movementVars = scene.movementComponents.at(entityID);
	glm::mat4& transform = scene.transformComponents.at(entityID);

	movementVars.moveSpeed = 0.1f;
	movementVars.lookSensitivity = 0.005f;

	transform = glm::inverse(glm::lookAt(pos, center, glm::vec3{ 0, 1, 0 }));

	return entityID;
}

const std::vector<VertexFormat>& GLUtils::getQuadVertices()
{
	static const std::vector<VertexFormat> vertices = {
		{ { -1,  1, 0 }, { -1, 1, 1 }, { 0, 1 } }, // Top left
		{ {  1,  1, 0 }, { 1, 1, 1 }, { 1, 1 } }, // Top right
		{ {  1, -1, 0 }, { 1, -1, 1 }, { 1, 0 } }, // Bottom right
		{ { -1, -1, 0 }, { -1, -1, 1 }, { 0, 0 } }  // Bottom left
	};

	return vertices;
}

const std::vector<GLuint>& GLUtils::getQuadIndices()
{
	static const std::vector<GLuint> indices = {
		0, 1, 2,
		0, 2, 3
	};

	return indices;
}

GLuint GLUtils::getDefaultShader()
{
	static GLuint shader;
	static bool shaderBuilt = false;

	if (!shaderBuilt) {
		compileAndLinkShaders(
			"Assets/Shaders/default_vert.glsl",
			"Assets/Shaders/default_frag.glsl",
			shader);
		shaderBuilt = true;
	}

	return shader;
}

GLuint GLUtils::bufferVertices(const std::vector<VertexFormat>& vertices, const std::vector<GLuint>& indices)
{
	GLuint VAO;
	GLuint buffers[2];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &buffers[0]);
	glGenBuffers(1, &buffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	
	GLuint positionLoc = 0;
	GLuint normalLoc = 1;
	GLuint texCoordLoc = 2;
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), BUFFER_OFFSET(3));
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), BUFFER_OFFSET(6));

	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(normalLoc);
	glEnableVertexAttribArray(texCoordLoc);

	return VAO;
}
