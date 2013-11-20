#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

/// A class for handling the window
class Window
{
public:
	//Window();
	//~Window();

	void initialize();
	void run();
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

#endif