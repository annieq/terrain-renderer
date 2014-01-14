#include "window.h"

Window::Window()
	: m_keys(NULL), m_renderer(NULL)
{
}

bool Window::initialize()
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	ApplicationHandle = this;

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
                          LPCWSTR(TEXT(APPTITLE)),   // title of the window
						  WS_OVERLAPPEDWINDOW,	// window style
						  //WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP /*| WS_MAXIMIZE */,    // window style - without frames
                          100,    // x-position of the window
                          100,    // y-position of the window
                          WINDOW_WIDTH,    // width of the window
						  WINDOW_HEIGHT,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
						  GetModuleHandle(NULL),    // application handle
                          NULL);    // used with multiple windows, NULL

	ShowWindow(hWindow, SW_SHOW);

	m_renderer = new Renderer();
	if (!m_renderer->initDX(hWindow))
		return false;

	m_keys = new Keys();
	if(!m_keys)
		return false;

	m_keys->Initialize();
	
	return true;
}

void Window::run()
{
	MSG msg;
	OPENFILENAME ofn;
	std::string str;
	char szFile[260];       // buffer for file name
	HANDLE hf;              // file handle
	
	ZeroMemory(&msg, sizeof(MSG));

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWindow;
	ofn.lpstrFile = (LPWSTR)szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Pliki terenu\0*.TER\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	std::string bslash = "\\";
	std::string dbslash = "\\\\";

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
			// Calculating camera moves
			D3DXVECTOR3 move(0.0f,0.0f,0.0f);
			D3DXVECTOR3 rotate(0.0f,0.0f,0.0f);
			float vertexMove = 0.0f;
			m_keys->checkMoveRotate(move, rotate,vertexMove);
			switch (m_keys->checkF())
			{
			case 2:
					m_keys->KeyReleased(F2);
				ofn.lpstrTitle = _T("Zapisz plik");
				ofn.Flags = OFN_OVERWRITEPROMPT;
				if(GetSaveFileName(&ofn) == TRUE) {
					str = CW2A(ofn.lpstrFile);
					for(size_t i=0;i<str.size();++i)
						if(str[i] == bslash.c_str()[0]) {
							str.replace(i,1,dbslash);
							++i;
						}
					m_renderer->saveTerrain(str);
				}
				break;
			case 3:
				m_keys->KeyReleased(F3);
				ofn.lpstrTitle = _T("Otwórz plik");
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if(GetOpenFileName(&ofn) == TRUE) {
					str = CW2A(ofn.lpstrFile);
					for(size_t i=0;i<str.size();++i)
						if(str[i] == bslash.c_str()[0]) {
							str.replace(i,1,dbslash);
							++i;
						}
					m_renderer->loadTerrain(str);
				}
				break;
			case 4:
				m_renderer->loadTerrain();
				break;
			}

			bool shiftstate = false;
			bool lmbState = m_keys->isLmbPressed(shiftstate);
			// RENDERING
			m_renderer->moveVertex(vertexMove);
			m_renderer->renderFrame(move,rotate,lmbState,shiftstate);
		}
    }
}

void stringBSlashReplacer(std::string& in)
{
	for(int i=0;i<in.size();++i)
		if(in[i] == char("\\"))
			in[i] = char("/");
}

void Window::shutdown()
{
	if (m_keys != NULL)
		delete m_keys;
	m_renderer->shutdown();
}

LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_keys->KeyPressed((unsigned int)wparam);
			if (wparam == F1)
				m_renderer->changeWireframe();
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_keys->KeyReleased((unsigned int)wparam);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			m_keys->lmbPressed(wparam & MK_SHIFT);
		}

		case WM_LBUTTONUP:
		{
			if(wparam == 0)
				m_keys->lmbRelased();
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
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
	return ApplicationHandle->MessageHandler (hWnd, message, wParam, lParam);
}