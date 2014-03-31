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
#include <xnamath.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <assert.h>
#include <atlstr.h>
#include <atlconv.h>

#include "FW1FontWrapper.h"

#define APPTITLE "Terrain renderer v1.0"
// sta³e:
#define RADIANS_PER_DEGREES  0.0174532925f

#define CAM_MOVE 0.2f	// pojedynczy krok ruchu kamery
#define CAM_ROTATE 0.5f	// pojedynczy obrót kamery w stopniach
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOV (D3DX_PI/4.0f) // field of view

#define TERRAIN_STEP 0.2	// krok podnoszenia/opuszczania wierzcho³ka

#define SCREEN_NEAR 0.1f
#define SCREEN_DEPTH 1000.0f

#define FRAMES_PER_SECOND 50
#define SKIP_TICKS (1000 / FRAMES_PER_SECOND)

// math :P
#define PI 3.14159265358979323846

#endif