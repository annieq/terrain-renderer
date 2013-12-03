#include "window.h"

bool Window::initialize()
{
	HWND hWindow;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the window structure
	wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWindow = CreateWindowEx(NULL,
                          L"WindowClass",    // name of the window class
                          L"Terrain Renderer v. 1.0",   // title of the window
						  WS_OVERLAPPEDWINDOW,	// window style
						  //WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP /*| WS_MAXIMIZE */,    // window style - without frames
                          100,    // x-position of the window
                          100,    // y-position of the window
                          800,    // width of the window
                          600,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
						  GetModuleHandle(NULL),    // application handle
                          NULL);    // used with multiple windows, NULL

	ShowWindow(hWindow, SW_SHOW);

	m_renderer = new Renderer();
	if (!m_renderer->initDX(hWindow))
		return false;
	
	return true;
}

void Window::run()
{
	MSG msg;
	
	ZeroMemory(&msg, sizeof(MSG));

    while(true)
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// RENDERING
			m_renderer->renderFrame();
		}
    }
}

void Window::shutdown()
{
	m_renderer->shutdown();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);

	// zamiast tego returna - fcja do obs³ugi klawiszy
}