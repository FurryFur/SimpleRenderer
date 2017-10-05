#pragma once

struct Scene;
struct GLFWwindow;

class InputSystem {
public:
	InputSystem(GLFWwindow* window, Scene&);

	void update(size_t entityID);

private:
	void keyCallback(int key, int scancode, int action, int mods);

	Scene& m_scene;
	GLFWwindow* m_window;
	bool m_leftMoveBtnDown;
	bool m_rightMoveBtnDown;
	bool m_forwardMoveBtnDown;
	bool m_backwardMoveBtnDown;
	bool m_downMoveBtnDown;
	bool m_upMoveBtnDown;
};