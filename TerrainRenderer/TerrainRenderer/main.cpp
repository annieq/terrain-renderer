#include "window.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Window w;

	if (w.initialize())
		w.run();
	else
	{
		MessageBox(NULL, L"Wyst�pi� b��d.", L"B��d", MB_OK);
	}

	w.shutdown();
}