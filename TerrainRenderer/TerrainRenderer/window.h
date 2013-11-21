#ifndef WINDOW_H
#define WINDOW_H

#include "renderer.h"

/// A class for handling the window
class Window
{
public:
	bool initialize();
	void run();
	void shutdown();

private:
	Renderer *m_renderer;
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

#endif