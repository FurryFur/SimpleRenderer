#include "Scene.h"
#include "Renderer.h"
#include "GLUtils.h"

#include <GLFW\glfw3.h>

int main()
{
	GLFWwindow* window = GLUtils::InitOpenGL();

	Scene scene;
	Renderer renderer(window, scene);
	//InputSystem inputSystem(scene);

	GLUtils::createQuad(scene, glm::mat4{1});

	size_t cameraEntity = GLUtils::createCamera(scene, { 0, 0, 2 }, { 0, 0, 0 }, { 0, 1, 0 });
	renderer.setCamera(cameraEntity);
	//size_t inputControllerEntity = inputSystem.createInputController();
	//inputSystem.possess(inputControllerEntity, cameraEntity);

	while (!glfwWindowShouldClose(window)) {
		renderer.renderFrame();
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}