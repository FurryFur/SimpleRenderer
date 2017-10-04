#include "GLUtils.h"

#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "Scene.h"
#include "ShaderHelper.h"
#include "Vertex.h"

#include <GLFW\glfw3.h>

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

GLFWwindow* GLUtils::InitOpenGL()
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

size_t GLUtils::createQuad(Scene& scene, glm::mat4 _transform)
{
	size_t entityID = scene.createEntity();
	size_t& componentMask = scene.getComponentMask(entityID);
	componentMask |= COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;

	// Get references to components
	glm::mat4& transform = scene.getTransformComponent(entityID);
	MeshComponent& mesh = scene.getMeshComponent(entityID);
	MaterialComponent& material = scene.getMaterialComponent(entityID);

	transform = _transform;

	material.shader = getDefaultShader();

	glGenVertexArrays(1, &mesh.VAO);
	std::vector<Vertex>& vertices = getQuadVertices();
	std::vector<GLuint>& indices = getQuadIndices();
	bufferVertices(mesh.VAO, vertices, indices);
	mesh.numIndices = indices.size();

	//material.shader = getDefaultShader();

	return entityID;
}

std::vector<Vertex>& GLUtils::getQuadVertices()
{
	static std::vector<Vertex> vertices = {
		{ { -0.5,  0.5f, 0 }, { 0, 0, 1 }, { 0, 1 } }, // Top left
		{ {  0.5,  0.5f, 0 }, { 0, 0, 1 }, { 1, 1 } }, // Top right
		{ {  0.5, -0.5f, 0 }, { 0, 0, 1 }, { 1, 0 } }, // Bottom right
		{ { -0.5, -0.5f, 0 }, { 0, 0, 1 }, { 0, 0 } }  // Bottom left
	};

	return vertices;
}

std::vector<GLuint>& GLUtils::getQuadIndices()
{
	static std::vector<GLuint> indices = {
		0, 2, 3,
		0, 1, 2
	};

	return indices;
}

GLuint GLUtils::getDefaultShader()
{
	GLuint shader;
	compileAndLinkShaders(
		"Assets/Shaders/default_vert.glsl",
		"Assets/Shaders/default_frag.glsl",
		shader);

	return shader;
}

void GLUtils::bufferVertices(GLuint VAO, std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	GLuint colorLoc = 0;
	GLuint normalLoc = 1;
	GLuint texCoordLoc = 2;
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(0));
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(3));
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(6));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}
