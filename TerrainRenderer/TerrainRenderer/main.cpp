//#include "common.h"
#include "window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Window w;

	if (w.initialize())
		w.run();

	w.shutdown();
}