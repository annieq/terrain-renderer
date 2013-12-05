#pragma once

#include "common.h"

class Keys {
public:
	Keys();
	~Keys();

	void Initialize();
	void Shutdown();
	bool Frame();

	void KeyPressed(unsigned int key);
	void KeyReleased(unsigned int key);
	bool isKeyPressed(unsigned int key);

	void checkMoveRotate(D3DXVECTOR3& move, D3DXVECTOR3& rotate);

private:
	bool m_keyboardState[256];

};