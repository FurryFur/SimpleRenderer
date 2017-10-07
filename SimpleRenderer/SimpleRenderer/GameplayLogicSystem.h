#pragma once

#include "KeyObserver.h"

struct Scene;
struct InputSystem;

class GameplayLogicSystem : IKeyObserver {
public:
	GameplayLogicSystem(Scene& scene, InputSystem& inputSystem);

	void update(size_t entityID);

	// Inherited via IKeyObserver
	virtual void keyCallback(int key, int scancode, int action, int mods) override;

private:
	Scene& m_scene;
	size_t m_oldPossessedEntity;
	size_t m_possessedEntity;
	bool m_oldPossessedEntityUpdated;
	bool m_newPossessedEntityUpdated;
};