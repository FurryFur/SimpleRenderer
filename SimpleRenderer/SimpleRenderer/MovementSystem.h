#pragma once

struct Scene;

class MovementSystem {
public:
	MovementSystem(Scene& scene);

	void update(size_t entityID);

private:
	Scene& m_scene;
};