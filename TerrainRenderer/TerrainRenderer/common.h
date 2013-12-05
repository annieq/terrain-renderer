#ifndef COMMON_H
#define COMMON_H


// komentarze linkera - wskazania na biblioteki
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <Windows.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <d3dx10math.h>
#include <vector>
#include <dinput.h>
//#include <dinput.h>

// sta³e:
#define RADIANS_PER_DEGREES  0.0174532925f

#define CAM_MOVE 1.0f	// pojedynczy krok ruchu kamery
#define CAM_ROTATE 2.0f	// pojedynczy obrót kamery w stopniach
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOV (D3DX_PI/4.0f) // field of view

#define SCREEN_NEAR 0.1f
#define SCREEN_DEPTH 1000.0f

#endif