#ifndef WINDOW_H
#define WINDOW_H

#include "renderer.h"
#include "keys.h"

#define SAVE_FILE true
#define LOAD_FILE false

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
	std::string openFile(bool save);

	Renderer *m_renderer;
	Keys *m_keys;
	HWND hWindow;

};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

static Window* ApplicationHandle = 0;

#endif