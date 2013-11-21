#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"

/// A renderer class (using DX)
class Renderer
{
public:
	Renderer();
	bool initializeDX(HWND);
	void shutdown();

private:
	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
};

#endif