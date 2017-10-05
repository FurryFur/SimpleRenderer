#include "InputSystem.h"

#include "InputComponent.h"
#include "Scene.h"

#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

InputSystem::InputSystem(GLFWwindow* window, Scene& scene)
	: m_window{ window }
	, m_scene{ scene }
{
	// Register input system as a listener for keyboard events
	glfwSetWindowUserPointer(window, this);
	auto func = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		InputSystem* inputSystem = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
		inputSystem->keyCallback(key, scancode, action, mods);

		// Close window on exit
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	};
	glfwSetKeyCallback(window, func);
}

void InputSystem::keyCallback(int key, int scancode, int action, int mods)
{
	// TODO: These mappings should not be hard coded here. Should instead be mapped in input component on entity creation
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_RELEASE))
		m_leftMoveBtnDown = (action == GLFW_PRESS);
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_RELEASE))
		m_rightMoveBtnDown = (action == GLFW_PRESS);
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_RELEASE))
		m_forwardMoveBtnDown = (action == GLFW_PRESS);
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_RELEASE))
		m_backwardMoveBtnDown = (action == GLFW_PRESS);
	if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_RELEASE))
		m_downMoveBtnDown = (action == GLFW_PRESS);
	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_RELEASE))
		m_upMoveBtnDown = (action == GLFW_PRESS);
}

void InputSystem::update(size_t entityID)
{
	// Filter input receivers
	const size_t kInputReceiverMask = COMPONENT_INPUT;
	if ((m_scene.componentMasks.at(entityID) & kInputReceiverMask) != kInputReceiverMask)
		return;

	// Update input from buttons
	InputComponent& input = m_scene.inputComponents.at(entityID);
	input.moveAxis = glm::vec3{ 0 };
	if (m_leftMoveBtnDown)
		input.moveAxis.x -= 1;
	if (m_rightMoveBtnDown)
		input.moveAxis.x += 1;
	if (m_forwardMoveBtnDown)
		input.moveAxis.z -= 1;
	if (m_backwardMoveBtnDown)
		input.moveAxis.z += 1;
	if (m_downMoveBtnDown)
		input.moveAxis.y -= 1;
	if (m_upMoveBtnDown)
		input.moveAxis.y += 1;

	// Update input from mouse
	static glm::dvec2 lastCursorPos;
	static bool firstRun = true;
	if (firstRun) {
		glfwGetCursorPos(m_window, &lastCursorPos.x, &lastCursorPos.y);
		firstRun = false;
	}
	glm::dvec2 cursorPos;
	glfwGetCursorPos(m_window, &cursorPos.x, &cursorPos.y);

	input.lookDelta = cursorPos - lastCursorPos;

	lastCursorPos = cursorPos;
}