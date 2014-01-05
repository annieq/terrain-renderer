#include "renderer.h"

Renderer::Renderer()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_camera = 0;

	m_FW1Factory = 0;
	m_FontWrapper = 0;

	m_backBuffer = 0;
	m_renderTargetView = 0;
	m_vBuffer = m_iBuffer = m_mBuffer = 0;
	m_rastSolid = m_rastWire = 0;

	m_textures = Textures();
	m_shader = Shader();
	m_wireframe = false;
	m_NextGameTick = GetTickCount();	// init next frame time for "now"
	m_terr = 0;
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

	// rasterizer state
	if (!createRasterizerState())
		return false;

	// viewport
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	m_viewport.Width = WINDOW_WIDTH;
	m_viewport.Height = WINDOW_HEIGHT;
	m_viewport.TopLeftX = 0.0;
	m_viewport.TopLeftY = 0.0;
	m_deviceContext->RSSetViewports(1, &m_viewport);

	if (!m_textures.init(m_device, m_deviceContext))
		return false;
	m_shader.init(m_device, m_deviceContext, m_textures.getTextures(), m_textures.getSamplerState());


	// Create instance of a camera
	m_camera = new Camera();
	if(!m_camera)
		return false;
	
	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, (float)FOV, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, (float)SCREEN_NEAR, (float)SCREEN_DEPTH);

    // Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&m_worldMatrix);
	
	// draw a terrain (actually set vertex & index buffers :P)
	m_terr = new Terrain(m_device);
	if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
		return false;
	if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
		return false;
	
	hr = FW1CreateFactory(FW1_VERSION, &m_FW1Factory);
	if (FAILED(hr))
		return false;
	hr = m_FW1Factory->CreateFontWrapper(m_device, L"Arial", &m_FontWrapper);
	if (FAILED(hr))
		return false;

	return true;
}

bool Renderer::createRasterizerState()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rastDesc;			//struct used to initialize the rasterizer state

	//define the rasterizer state object config
	rastDesc = D3D11_RASTERIZER_DESC();		//reset struct
	
	rastDesc.AntialiasedLineEnable = false;
	rastDesc.CullMode = D3D11_CULL_BACK;		//use D3D11_CULL_BACK/FRONT to actually do any "backface" culling
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = true;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = false;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0.0f;

	//create the rasterizer state object
	hr = m_device->CreateRasterizerState(&rastDesc, &m_rastSolid);
	if (FAILED(hr))
		return false;

	//modify the fill mode only
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;

	//create the rasterizer state object (wireframe)
	hr = m_device->CreateRasterizerState(&rastDesc, &m_rastWire);
	if (FAILED(hr))
		return false;

	m_deviceContext->RSSetState(m_rastSolid);

	return true;
}
	
/*
void Renderer::GetProjectionMatrix(D3DXMATRIX& projMatrix) {
	projMatrix = m_projectionMatrix;
}
void Renderer::GetWorldMatrix(D3DXMATRIX& worldMatrix) {
	worldMatrix = m_worldMatrix;

}
*/

void Renderer::renderFrame(D3DXVECTOR3 move, D3DXVECTOR3 rotate, bool lmbState)
{
	int sleep_time = 0;
	HRESULT hr;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, D3DXCOLOR(0.01f, 0.01f, 0.01f, 1.0f));

	// move and rotate the camera
	m_camera->Move(move);
	m_camera->Rotate(rotate);
	
	// Create Constant Buffers and send world and viewprojection matrix through it
	VS_CONSTANT_BUFFER mConstData;

	mConstData.worldMatrix = m_worldMatrix;
	m_camera->GetViewMatrix(m_viewMatrix);
	D3DXMatrixMultiply(&(mConstData.viewProjMatrix), &m_viewMatrix, &m_projectionMatrix);

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof( VS_CONSTANT_BUFFER );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &mConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer. (TU BY TRZEBA ZWRACAC HR)
	hr = m_device->CreateBuffer( &cbDesc, &InitData, &m_mBuffer );


	// Set the buffer.
	m_deviceContext->VSSetConstantBuffers( 0, 1, &m_mBuffer );

	// configure Input Assembler stage
	UINT stride = sizeof(Vertex_PosTex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vBuffer, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_iBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	m_deviceContext->RSSetState(m_wireframe ? m_rastWire : m_rastSolid);


	if(lmbState)
	{
		POINT mouseCoords;
		GetCursorPos(&mouseCoords);	// get mouse coordinates (x,y)
		float x = (2.0f * mouseCoords.x) / WINDOW_WIDTH - 1.0f;	// Normalise them
		float y = 1.0f - (2.0f * mouseCoords.y) / WINDOW_HEIGHT;
		float z = 1.0f;
		D3DXVECTOR4 origin(x,y,0,1);
		D3DXVECTOR4 end(x,y,1,1);

		D3DXMATRIX *inverseviewprojmx = new D3DXMATRIX();
		D3DXMatrixInverse(inverseviewprojmx,NULL,&(mConstData.viewProjMatrix));
		D3DXVECTOR4 *origin4 = new D3DXVECTOR4();
		D3DXVECTOR4 *end4 = new D3DXVECTOR4();
		D3DXVec4Transform(origin4,&origin,inverseviewprojmx);
		D3DXVec4Transform(end4,&end,inverseviewprojmx);

		m_terr->checkPoints(origin4,end4);
	}
	// draw vertices
	if (m_numberOfVertices)
	{
		if (m_numberOfIndices)
			m_deviceContext->DrawIndexed(m_numberOfIndices, 0, 0);
		else
			m_deviceContext->Draw(m_numberOfVertices, 0);
	}
	

	// prepare the text to draw
	std::wstringstream oss;
	D3DXVECTOR3 cpos, crot;
	cpos = m_camera->GetPosition();
	crot = m_camera->GetRotation();
	oss << APPTITLE << std::endl;
	oss << "Pozycja kamery: x=" << cpos.x << " y=" << cpos.y << " z=" << cpos.z << std:: endl;
	oss << "Obrót kamery: x=" << crot.x << " y=" << crot.y << " z=" << crot.z << std:: endl;
	oss << "Klawisze: \nStrzalki - ruch kamery :: PageUp/Down - zblizenie/oddalenie :: NUM2/4/6/8 - obrót kamery" << std:: endl;
	oss << "F1 - tryb wireframe" << std::endl;
	oss << "Stan LMB: " << (lmbState?"true":"false") << std::endl;

	std::wstring text = oss.str();

	m_FontWrapper->DrawString(
		m_deviceContext,
		LPCTSTR(text.c_str()),// String
		14.0f,// Font size
		10.0f,// X position
		10.0f,// Y position
		0xffeeeeee,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);

	m_swapChain->Present(0, 0);
	
	// FPS Limiter
	m_NextGameTick += SKIP_TICKS;
	sleep_time = m_NextGameTick - GetTickCount();
	if( sleep_time >= 0 ) {
            Sleep( sleep_time );
    }
}

void Renderer::changeWireframe()
{
	m_wireframe = !m_wireframe;
}

void Renderer::shutdown()
{
	if (m_camera)
		delete m_camera;
	m_shader.release();
	m_textures.release();

	if (m_terr)
		delete m_terr;
	if (m_swapChain)
		m_swapChain->Release();
	if (m_renderTargetView)
		m_renderTargetView->Release();
	if (m_backBuffer)
		m_backBuffer->Release();
	if (m_FW1Factory)
		m_FW1Factory->Release();
	if (m_FontWrapper)
		m_FontWrapper->Release();
	if (m_vBuffer)
		m_vBuffer->Release();
	if (m_iBuffer)
		m_iBuffer->Release();
	if (m_mBuffer)
		m_mBuffer->Release();
	if (m_rastSolid)
		m_rastSolid->Release();
	if (m_rastWire)
		m_rastWire->Release();
	if (m_device)
		m_device->Release();
	if (m_deviceContext)
		m_deviceContext->Release();
}

