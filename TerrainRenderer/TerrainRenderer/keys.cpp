#include "keys.h"

Keys::Keys()
{
}

Keys::~Keys()
{
}


void Keys::Initialize()
{
	for(int i=0; i<256; i++)
		m_keyboardState[i] = false;
}


void Keys::KeyPressed(unsigned int key)
{
	m_keyboardState[key] = true;
}

void Keys::KeyReleased(unsigned int key)
{
	m_keyboardState[key] = false;
}

bool Keys::isKeyPressed(unsigned int key)
{
	// Do a bitwise and on the keyboard state to check if 'key' is currently being pressed.
	return m_keyboardState[key];
}

void Keys::checkMoveRotate(D3DXVECTOR3& move, D3DXVECTOR3& rotate)
{
	move.x -= m_keyboardState[LEFTARROW] ? 1 : 0;
	move.x += m_keyboardState[RIGHTARROW] ? 1 : 0;
	move.y += m_keyboardState[UPARROW] ? 1 : 0;
	move.y -= m_keyboardState[DOWNARROW] ? 1 : 0;
	move.z += m_keyboardState[PGUP] ? 1 : 0;
	move.z -= m_keyboardState[PGDN] ? 1 : 0;
	
	rotate.y -= m_keyboardState[NLEFTARROW] ? 1 : 0;
	rotate.y += m_keyboardState[NRIGHTARROW] ? 1 : 0;
	rotate.x -= m_keyboardState[NUPARROW] ? 1 : 0;
	rotate.x += m_keyboardState[NDOWNARROW] ? 1 : 0;
}