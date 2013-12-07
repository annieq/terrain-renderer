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
#include <assert.h>

// sta³e:
#define RADIANS_PER_DEGREES  0.0174532925f

#define CAM_MOVE 0.005f	// pojedynczy krok ruchu kamery (PRZYDA£BY SIÊ OGRANICZNIK FPS)
#define CAM_ROTATE 0.05f	// pojedynczy obrót kamery w stopniach
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOV (D3DX_PI/4.0f) // field of view

#define SCREEN_NEAR 0.1f
#define SCREEN_DEPTH 1000.0f

#define LEFTARROW 37
#define UPARROW 38
#define RIGHTARROW 39
#define DOWNARROW 40

#define NDOWNARROW 98
#define NLEFTARROW 100
#define NRIGHTARROW 102
#define NUPARROW 104

#define PGUP 33
#define PGDN 34

#endif