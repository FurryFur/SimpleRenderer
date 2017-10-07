#include "GLUtils.h"
#include "SceneUtils.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "GameplayLogicSystem.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

int main()
{
	GLFWwindow* window = GLUtils::initOpenGL();

	Scene scene;
	RenderSystem renderSystem(window, scene);
	MovementSystem movementSystem(scene);
	InputSystem inputSystem(window, scene);
	GameplayLogicSystem gameplayLogicSystem(scene, inputSystem);

	SceneUtils::createQuad(scene, glm::translate({}, glm::vec3{-1.5f, 1.5f, 0}));
	SceneUtils::createSphere(scene, glm::translate({}, glm::vec3{ 1.5f, 1.5f, 0 }));
	SceneUtils::createSphere(scene, glm::translate({}, glm::vec3{ -1.5f, -1.5f, 0 }));
	SceneUtils::createQuad(scene, glm::translate({}, glm::vec3{ 1.5f, -1.5f, 0 }));

	size_t cameraEntity = SceneUtils::createCamera(scene, { 0, 0, 6 }, { 0, 0, 0 }, { 0, 1, 0 });
	renderSystem.setCamera(cameraEntity);

	while (!glfwWindowShouldClose(window)) {
		inputSystem.beginFrame();
		renderSystem.beginRender();

		for (size_t entityID = 0; entityID < SceneUtils::getEntityCount(scene); ++entityID) {
			gameplayLogicSystem.update(entityID);
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