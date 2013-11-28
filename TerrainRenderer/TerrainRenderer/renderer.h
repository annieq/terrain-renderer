#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include <d3dx9math.h>

/// A renderer class (using DX)
class Renderer
{
public:
	Renderer();
	bool initializeDX(HWND);
	void renderFrame();
	void shutdown();

private:
	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	ID3D11Texture2D *m_backBuffer;
	ID3D11RenderTargetView *m_renderTargetView;
	D3D11_VIEWPORT m_viewport;
};

#endif