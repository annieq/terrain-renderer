#include "window.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Window w;

	if (w.initialize())
		w.run();
	else
	{
		MessageBox(NULL, L"Wyst¹pi³ b³¹d.", L"B³¹d", MB_OK);
	}

	w.shutdown();
}