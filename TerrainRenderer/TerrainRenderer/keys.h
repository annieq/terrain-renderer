#pragma once

#include "common.h"

#define LEFTARROW 37
#define UPARROW 38
#define RIGHTARROW 39
#define DOWNARROW 40

#define NDOWNARROW 98
#define NLEFTARROW 100
#define NRIGHTARROW 102
#define NUPARROW 104

#define NUMPADPLUS 107
#define NUMPADMINUS 109

#define PGUP 33
#define PGDN 34

#define F1 112
#define F2 113
#define F3 114
#define F4 115

#define F5 116
#define F6 117
#define F7 118
#define F8 119

// ....
// #define F12 123

#define ESC 27

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

	void lmbPressed(bool shiftStatus);
	void lmbRelased();
	bool isLmbPressed(bool& shiftStatus);

	void checkMoveRotate(D3DXVECTOR3& move, D3DXVECTOR3& rotate, float& vertexMove);
	int checkF();	// check if functional button is pressed

private:
	bool m_keyboardState[256];
	bool m_lmbstate;
	bool m_shiftstate;
};