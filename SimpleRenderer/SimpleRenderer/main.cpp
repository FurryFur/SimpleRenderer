#include "Scene.h"
#include "RenderSystem.h"
//#include "MovementSystem.h"
#include "GLUtils.h"

#include <GLFW\glfw3.h>

int main()
{
	GLFWwindow* window = GLUtils::InitOpenGL();

	Scene scene;
	RenderSystem renderSystem(window, scene);
	//MovementSystem movementSystem(scene);

	GLUtils::createQuad(scene, glm::mat4{1});

	size_t cameraEntity = GLUtils::createCamera(scene, { 0, 0, 2 }, { 0, 0, 0 }, { 0, 1, 0 });
	renderSystem.setCamera(cameraEntity);
	//size_t inputControllerEntity = inputSystem.createInputController();
	//inputSystem.possess(inputControllerEntity, cameraEntity);


	while (!glfwWindowShouldClose(window)) {
		renderSystem.beginRender();

		for (size_t entityID = 0; entityID < scene.getEntityCount(); ++entityID) {
			renderSystem.update(entityID);
		}
		
		renderSystem.endRender();
		
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}