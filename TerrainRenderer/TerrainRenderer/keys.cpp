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
	m_lmbstate = false;
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
	return m_keyboardState[key];
}

void Keys::lmbPressed(bool shiftStatus)
{
	m_lmbstate = true;
	m_shiftstate = shiftStatus;
}

void Keys::lmbRelased()
{
	m_lmbstate = false;
	m_shiftstate = false;
}

bool Keys::isLmbPressed(bool& shiftStatus)
{
	shiftStatus = m_shiftstate;
	return m_lmbstate;
}

void Keys::checkMoveRotate(D3DXVECTOR3& move, D3DXVECTOR3& rotate, float& vertexMove)
{
	move.x -= m_keyboardState[LEFTARROW] ? 1 : 0;
	move.x += m_keyboardState[RIGHTARROW] ? 1 : 0;
	move.z += m_keyboardState[UPARROW] ? 1 : 0;
	move.z -= m_keyboardState[DOWNARROW] ? 1 : 0;
	move.y += m_keyboardState[PGUP] ? 1 : 0;
	move.y -= m_keyboardState[PGDN] ? 1 : 0;
	
	rotate.y -= m_keyboardState[NLEFTARROW] ? 1 : 0;
	rotate.y += m_keyboardState[NRIGHTARROW] ? 1 : 0;
	rotate.x -= m_keyboardState[NUPARROW] ? 1 : 0;
	rotate.x += m_keyboardState[NDOWNARROW] ? 1 : 0;
	
	vertexMove += m_keyboardState[NUMPADPLUS] ? TERRAIN_STEP : 0.0f;
	vertexMove -= m_keyboardState[NUMPADMINUS] ? TERRAIN_STEP : 0.0f;
}

int Keys::checkF()
{
	if (m_keyboardState[F1])
		return 1;
	if (m_keyboardState[F2])
		return 2;
	if (m_keyboardState[F3])
		return 3;
	if (m_keyboardState[F4])
		return 4;
	if (m_keyboardState[F5])
		return 5;
	if (m_keyboardState[F6])
		return 6;
	if (m_keyboardState[F7])
		return 7;
	if (m_keyboardState[F8])
		return 8;

	return -1;
}