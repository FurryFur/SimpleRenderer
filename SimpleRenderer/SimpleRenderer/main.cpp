#include "GLUtils.h"
#include "SceneUtils.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "Scene.h"

#include <GLFW\glfw3.h>

int main()
{
	GLFWwindow* window = GLUtils::initOpenGL();

	Scene scene;
	RenderSystem renderSystem(window, scene);
	MovementSystem movementSystem(scene);
	InputSystem inputSystem(window, scene);

	SceneUtils::createQuad(scene, glm::mat4{ 1 });
	SceneUtils::createSphere(scene, glm::mat4{ 1 });

	size_t cameraEntity = SceneUtils::createCamera(scene, { 0, 0, 2 }, { 0, 0, 0 }, { 0, 1, 0 });
	renderSystem.setCamera(cameraEntity);

	while (!glfwWindowShouldClose(window)) {
		inputSystem.beginFrame();
		renderSystem.beginRender();

		for (size_t entityID = 0; entityID < SceneUtils::getEntityCount(scene); ++entityID) {
			inputSystem.update(entityID);
			movementSystem.update(entityID);
			renderSystem.update(entityID);
		}
		
		renderSystem.endRender();
		
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}