#include "InputSystem.h"

#include "Scene.h"

#include <GLFW\glfw3.h>

InputSystem::InputSystem(GLFWwindow* window, Scene& scene)
	: m_window{ window }
	, m_scene{ scene }
{
	// Register input system as a listener for keyboard events
	glfwSetWindowUserPointer(window, this);
	auto keyFunc = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		//InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		//inputSystem->keyCallback(key, scancode, action, mods);

		// Close window on exit
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	};
	glfwSetKeyCallback(window, keyFunc);

	// Register input system as a listener for mouse move events
	//auto mouseFunc = [](GLFWwindow* window, double xpos, double ypos) {
	//	InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
	//	inputSystem->mouseMoveCallback(glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) });
	//};
	//glfwSetCursorPosCallback(window, mouseFunc);
}

void InputSystem::beginFrame()
{
	static glm::dvec2 lastMousePos;

	// Set previous mouse pos to current mouse pos on first run
	static bool firstRun = true;
	if (firstRun) {
		glfwGetCursorPos(m_window, &lastMousePos.x, &lastMousePos.y);
		firstRun = false;
	}

	// Update input from mouse
	glm::dvec2 mousePos;
	glfwGetCursorPos(m_window, &mousePos.x, &mousePos.y);

	// Update mouse delta
	m_mouseDelta = mousePos - lastMousePos;

	lastMousePos = mousePos;
}

//void InputSystem::keyCallback(int key, int scancode, int action, int mods)
//{
//	// TODO: These mappings should not be hard coded here. Should instead be mapped in input component on entity creation
//	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_RELEASE))
//		m_leftBtnDown = (action == GLFW_PRESS);
//	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_RELEASE))
//		m_rightBtnDown = (action == GLFW_PRESS);
//	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_RELEASE))
//		m_forwardBtnDown = (action == GLFW_PRESS);
//	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_RELEASE))
//		m_backwardBtnDown = (action == GLFW_PRESS);
//	if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_RELEASE))
//		m_downBtnDown = (action == GLFW_PRESS);
//	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_RELEASE))
//		m_upBtnDown = (action == GLFW_PRESS);
//}

void InputSystem::update(size_t entityID)
{
	// Filter input receivers
	const size_t kInputReceiverMask = COMPONENT_INPUT;
	if ((m_scene.componentMasks.at(entityID) & kInputReceiverMask) != kInputReceiverMask)
		return;

	InputComponent& input = m_scene.inputComponents.at(entityID);

	// Update input from mouse
	input.orientationDelta = {};
	if (input.mouseInputEnabled)
		input.orientationDelta = glm::vec3{ m_mouseDelta, 0 };

	// Update input from buttons
	input.axis = {};
	if (input.leftBtnMap && glfwGetKey(m_window, input.leftBtnMap) == GLFW_PRESS)
		input.axis.x -= 1;
	if (input.rightBtnMap && glfwGetKey(m_window, input.rightBtnMap) == GLFW_PRESS)
		input.axis.x += 1;
	if (input.forwardBtnMap && glfwGetKey(m_window, input.forwardBtnMap) == GLFW_PRESS)
		input.axis.z -= 1;
	if (input.backwardBtnMap && glfwGetKey(m_window, input.backwardBtnMap) == GLFW_PRESS)
		input.axis.z += 1;
	if (input.downBtnMap && glfwGetKey(m_window, input.downBtnMap) == GLFW_PRESS)
		input.axis.y -= 1;
	if (input.upBtnMap && glfwGetKey(m_window, input.upBtnMap) == GLFW_PRESS)
		input.axis.y += 1;
	if (input.azimuthPosBtnMap && glfwGetKey(m_window, input.azimuthPosBtnMap) == GLFW_PRESS)
		input.orientationDelta.x += 1;
	if (input.azimuthNegBtnMap && glfwGetKey(m_window, input.azimuthNegBtnMap) == GLFW_PRESS)
		input.orientationDelta.x -= 1;
	if (input.elevationPosBtnMap && glfwGetKey(m_window, input.elevationPosBtnMap) == GLFW_PRESS)
		input.orientationDelta.y += 1;
	if (input.elevationNegBtnMap && glfwGetKey(m_window, input.elevationNegBtnMap) == GLFW_PRESS)
		input.orientationDelta.y -= 1;
	if (input.rollBtnMap && glfwGetKey(m_window, input.rollBtnMap) == GLFW_PRESS)
		input.orientationDelta.z += 1;
}