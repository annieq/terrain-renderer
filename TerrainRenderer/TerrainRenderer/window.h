#ifndef WINDOW_H
#define WINDOW_H

#include "renderer.h"
#include "keys.h"

/// A class for handling the window
class Window
{
public:
	Window();
	bool initialize();
	void run();
	void shutdown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	void initOFN(OPENFILENAME &ofn);

	Renderer *m_renderer;
	Keys *m_keys;
	HWND hWindow;

};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

static Window* ApplicationHandle = 0;

#endif