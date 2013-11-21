#include "renderer.h"

Renderer::Renderer()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
}

bool Renderer::initializeDX(HWND hWnd)
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
    swapChainDesc.BufferCount = 1;                                  // one back buffer
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // use 32-bit color
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how swap chain is to be used
    swapChainDesc.OutputWindow = hWnd;                              // the window to be used
    swapChainDesc.SampleDesc.Count = 1;                             // how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;                                  // windowed/full-screen mode

	//D3D_FEATURE_LEVEL featureLvl = D3D_FEATURE_LEVEL_9_1;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,//&featureLvl, 1,
				D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(hr))
		return false;

	// TODO: inne inicjalizacyjne rzeczy

	return true;
}

void Renderer::shutdown()
{
	if (m_swapChain)
		m_swapChain->Release();
	if (m_device)
		m_device->Release();
	if (m_deviceContext)
		m_deviceContext->Release();
}