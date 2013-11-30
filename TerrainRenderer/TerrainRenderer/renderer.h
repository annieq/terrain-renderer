#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include <d3dx9math.h>
#include "shader.h"
#include <vector>

/// A renderer class (using DX)
class Renderer
{
public:
	Renderer();
	bool initDX(HWND);
	void renderFrame();
	void shutdown();

private:
	bool drawFigure();

private:
	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	ID3D11Texture2D *m_backBuffer;
	ID3D11RenderTargetView *m_renderTargetView;
	D3D11_VIEWPORT m_viewport;
	ID3D11Buffer *m_vBuffer;

	unsigned int m_numberOfVertices;

	Shader shader;
};
struct VERTEX
{
      FLOAT X, Y, Z;      // position
      D3DXCOLOR Color;    // color
};

#endif