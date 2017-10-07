#pragma once

#include <glm\glm.hpp>

#include <functional>
#include <vector>

struct Scene;
struct GLFWwindow;
class IKeyObserver;

class InputSystem {
public:
	InputSystem(GLFWwindow* window, Scene&);

	void update(size_t entityID);
	void beginFrame();

	void registerKeyObserver(IKeyObserver* observer);

private:
	void keyCallback(int key, int scancode, int action, int mods);

	Scene& m_scene;
	GLFWwindow* m_window;
	glm::dvec2 m_mouseDelta;
	std::vector<IKeyObserver*> m_keyObservers;
};