#pragma once

class IKeyObserver {
public:
	virtual void keyCallback(int key, int scancode, int action, int mods) = 0;
};