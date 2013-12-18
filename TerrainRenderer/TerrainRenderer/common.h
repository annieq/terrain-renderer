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
#include <sstream>
#include <string.h>
#include <assert.h>

#include "FW1FontWrapper.h"

#define APPTITLE "Terrain renderer v1.0"
// sta�e:
#define RADIANS_PER_DEGREES  0.0174532925f

#define CAM_MOVE 0.005f	// pojedynczy krok ruchu kamery (PRZYDA�BY SI� OGRANICZNIK FPS)
#define CAM_ROTATE 0.05f	// pojedynczy obr�t kamery w stopniach
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOV (D3DX_PI/4.0f) // field of view

#define SCREEN_NEAR 0.1f
#define SCREEN_DEPTH 1000.0f

#endif