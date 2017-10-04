#include "Scene.h"
#include "Renderer.h"
#include "GLUtils.h"

#include <GLFW\glfw3.h>

int main()
{
	GLFWwindow* window = GLUtils::InitOpenGL();

	Scene scene;
	Renderer renderer(scene);

	GLUtils::createQuad(scene, glm::mat4{1});

	while (!glfwWindowShouldClose(window)) {
		renderer.renderFrame();
	}

}