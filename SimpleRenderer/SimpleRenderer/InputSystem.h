#pragma once

#include <glm\glm.hpp>

struct Scene;
struct GLFWwindow;

class InputSystem {
public:
	InputSystem(GLFWwindow* window, Scene&);

	void update(size_t entityID);
	void beginFrame();

private:
	//void keyCallback(int key, int scancode, int action, int mods);

	Scene& m_scene;
	GLFWwindow* m_window;
	glm::dvec2 m_mouseDelta;
	//bool m_leftBtnDown;
	//bool m_rightBtnDown;
	//bool m_forwardBtnDown;
	//bool m_backwardBtnDown;
	//bool m_downBtnDown;
	//bool m_upBtnDown;
};