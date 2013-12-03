#include "renderer.h"

Renderer::Renderer()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
}

bool Renderer::initDX(HWND hWnd)
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

	// back buffer & render target
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backBuffer);
	if (FAILED(hr))
		return false;
	hr = m_device->CreateRenderTargetView(m_backBuffer, NULL, &m_renderTargetView);
	if (FAILED(hr))
		return false;
	m_backBuffer->Release();	// we no longer need it

	// tu mo¿e byæ jeszcze np. inicjalizacja depthStencil

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);

	// viewport
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	m_viewport.Width = 800;
	m_viewport.Height = 600;
	m_viewport.TopLeftX = 0.0;
	m_viewport.TopLeftY = 0.0;
	m_deviceContext->RSSetViewports(1, &m_viewport);

	shader.init(m_device, m_deviceContext);

	// draw a terrain (actually set vertex & index buffers :P)
	Terrain terr = Terrain(m_device);
	if (!terr.createVertices(&m_vBuffer, &m_numberOfVertices))
		return false;
	if (!terr.createIndices(&m_iBuffer, &m_numberOfIndices))
		return false;

	return true;
}

//bool Renderer::drawFigure()
//{
//	HRESULT hr;
//
//	std::vector<Vertex_PosCol> vertices;
//	std::vector<WORD> indices;
//
//	vertices.push_back(Vertex_PosCol(+0.5, +0.5, 0.0, D3DXCOLOR(1.0, 0.0, 1.0, 1.0) ));
//	vertices.push_back(Vertex_PosCol(-0.5, -0.5, 0.0, D3DXCOLOR(1.0, 1.0, 1.0, 1.0) ));
//	vertices.push_back(Vertex_PosCol(-0.5, +0.5, 0.0, D3DXCOLOR(1.0, 1.0, 1.0, 1.0) ));
//	vertices.push_back(Vertex_PosCol(+0.5, -0.5, 0.0, D3DXCOLOR(0.0, 0.0, 0.0, 1.0) ));
//	m_numberOfVertices = vertices.size();
//
//	indices.push_back(0);
//	indices.push_back(1);
//	indices.push_back(2);
//	indices.push_back(0);
//	indices.push_back(3);
//	indices.push_back(1);
//	m_numberOfIndices = indices.size();
//
//	// vertex buffer
//	D3D11_BUFFER_DESC bufDesc = D3D11_BUFFER_DESC();
//	bufDesc.Usage = D3D11_USAGE_DEFAULT;
//	bufDesc.ByteWidth = sizeof(Vertex_PosCol) * m_numberOfVertices;
//	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufDesc.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA bufData = D3D11_SUBRESOURCE_DATA();
//	bufData.pSysMem = &vertices[0];
//
//	hr = m_device->CreateBuffer(&bufDesc, &bufData, &m_vBuffer);
//	if (FAILED(hr))
//		return false;
//
//	// index buffer
//	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bufDesc.ByteWidth = sizeof(DWORD) * m_numberOfIndices;
//	bufData.pSysMem = &indices[0];
//
//	hr = m_device->CreateBuffer(&bufDesc, &bufData, &m_iBuffer);
//	if (FAILED(hr))
//		return false;
//
//	return true;
//}

void Renderer::renderFrame()
{
	/*************** lol *****************/
	static double cnt = 0;
	static bool dir = true;
	if (cnt > 1.0 || cnt < 0.0)
		dir = !dir;
	if (dir)
		cnt += 0.0001;
	else
		cnt -= 0.0001;
	/************ end of lol ************/
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, D3DXCOLOR(cnt, cnt, 0.3, 1.0));

	// configure Input Assembler stage
	UINT stride = sizeof(Vertex_PosCol);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vBuffer, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_iBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw vertices
	if (m_numberOfVertices)
	{
		if (m_numberOfIndices)
			m_deviceContext->DrawIndexed(m_numberOfIndices, 0, 0);
		else
			m_deviceContext->Draw(m_numberOfVertices, 0);
	}

	m_swapChain->Present(0, 0);
}

void Renderer::shutdown()
{
	if (m_swapChain)
		m_swapChain->Release();
	shader.release();
	if (m_renderTargetView)
		m_renderTargetView->Release();
	if (m_device)
		m_device->Release();
	if (m_deviceContext)
		m_deviceContext->Release();
}