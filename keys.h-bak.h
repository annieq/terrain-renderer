#pragma once

#include "common.h"

class Keys {
public:
	Keys();
	Keys(const Keys&);
	~Keys();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	// bool ReadMouse();
	void ProcessInput();

// The first three private member variables are the interfaces to Direct Input, the keyboard device, and the mouse device.

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	// IDirectInputDevice8* m_mouse;

// The next two private member variables are used for recording the current state of the keyboard and mouse devices.

	unsigned char m_keyboardState[256];
	// DIMOUSESTATE m_mouseState;

	// int m_screenWidth, m_screenHeight;
	// int m_mouseX, m_mouseY;

};