#include "renderer.h"
#include "keys.h"

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
	m_vBuffer = m_iBuffer = 0;
	m_rastSolid = m_rastWire = 0;

	m_textures = Textures();
	m_shader = Shader();
	m_wireframe = false;
	m_NextGameTick = GetTickCount();	// init next frame time for "now"
	m_terr = 0;
	m_depthStencil = NULL;
	m_depthStencilView = NULL;
	m_DSState = NULL;
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

	D3D_FEATURE_LEVEL featureLvl = D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,&featureLvl, 1,
				D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(hr))
		return false;

	m_windowHandle = hWnd;
	// back buffer & render target
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backBuffer);
	if (FAILED(hr))
		return false;
	// depthStencil texture initialization
	hr = createDepthScencilTexture();
	if (FAILED(hr))
		return false;

	hr = m_device->CreateRenderTargetView(m_backBuffer, NULL, &m_renderTargetView);
	if (FAILED(hr))
		return false;
	
	// depth-stencil view initialization
	hr = createDepthScencilView();
	if (FAILED(hr))
		return false;

	// rasterizer state
	if (!createRasterizerState())
		return false;


	// viewport
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	m_viewport.Width = WINDOW_WIDTH;
	m_viewport.Height = WINDOW_HEIGHT;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
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

	// init const buffers
	if (!createCBuffers())
		return false;
	
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
	
	m_backBuffer->Release();	// we no longer need it
	
	
	hr = createDepthScencilState();
	if (FAILED(hr))
		return false;

	m_deviceContext->OMSetDepthStencilState(m_DSState,1);

	// Bind the depth stencil view
	m_deviceContext->OMSetRenderTargets( 1,          // One rendertarget view
										&m_renderTargetView,      // Render target view, created earlier
										m_depthStencilView );     // Depth stencil view for the render targ

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return true;
}

bool Renderer::createCBuffers()
{
	HRESULT hr;
	// Create Constant Buffers
	VS_CBUF_MATRIX mConstData;

	// Fill in the matrix data
	mConstData.worldMatrix = m_worldMatrix;
	m_camera->GetViewMatrix(m_viewMatrix);
	D3DXMatrixMultiply(&(mConstData.viewProjMatrix), &m_viewMatrix, &m_projectionMatrix);

	// Fill in a buffer's description.
	m_cbufMatrixDesc.ByteWidth = sizeof( VS_CBUF_MATRIX );
	m_cbufMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_cbufMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_cbufMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_cbufMatrixDesc.MiscFlags = 0;
	m_cbufMatrixDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &mConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create and set the matrix buffer.
	ID3D11Buffer *cBuffer = 0;
	hr = m_device->CreateBuffer( &m_cbufMatrixDesc, &InitData, &cBuffer );
	if (FAILED(hr))
		return false;
	m_deviceContext->VSSetConstantBuffers( 0, 1, &cBuffer );
	cBuffer->Release();

	return true;
}

bool Renderer::changeTerrain(short type)
{
	if (type == F5)
	{
		m_terr->release();
		m_terr = new Terrain(m_device);
		if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
			return false;
		if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
			return false;
	}
	else if (type == F6)
	{
		m_terr->release();
		m_terr = new FaultForm(m_device);
		if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
			return false;
		if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
			return false;
	}
	else if (type == F7)
	{
		m_terr->release();
		m_terr = new ImprovedPerlinNoise(m_device);
		if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
			return false;
		if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
			return false;
	}
	else if (type == F8)
	{
		m_terr->release();
		m_terr = new DiamondSquare(m_device);
		if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
			return false;
		if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
			return false;
	}
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
	
HRESULT Renderer::createDepthScencilTexture()
{
	// depthStencil initialization
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth = D3D11_TEXTURE2D_DESC();	

	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	return m_device->CreateTexture2D( &descDepth, NULL, &m_depthStencil );
}

HRESULT Renderer::createDepthScencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV = D3D11_DEPTH_STENCIL_VIEW_DESC();
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	return m_device->CreateDepthStencilView( m_depthStencil, // Depth stencil texture
											 &descDSV, // Depth stencil desc
											 &m_depthStencilView );  // [out] Depth stencil view
}

HRESULT Renderer::createDepthScencilState()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	dsDesc = D3D11_DEPTH_STENCIL_DESC();
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;

	// Create depth stencil state
	return m_device->CreateDepthStencilState(&dsDesc, &m_DSState);
}

void Renderer::renderFrame(D3DXVECTOR3 move, D3DXVECTOR3 rotate, bool lmbState, bool shiftStatus)
{
	int sleep_time = 0;
	HRESULT hr;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, D3DXCOLOR(0.01f, 0.01f, 0.01f, 1.0f));
	m_deviceContext->ClearDepthStencilView(m_depthStencilView,D3D11_CLEAR_DEPTH  | D3D11_CLEAR_STENCIL,1.0,NULL);

	// move and rotate the camera
	m_camera->Move(move);
	m_camera->Rotate(rotate);
	
	// update constant buffer
	VS_CBUF_MATRIX mConstData;

	mConstData.worldMatrix = m_worldMatrix;
	m_camera->GetViewMatrix(m_viewMatrix);
	D3DXMatrixMultiply(&(mConstData.viewProjMatrix), &m_viewMatrix, &m_projectionMatrix);

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &mConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	ID3D11Buffer *cBuffer = 0;
	hr = m_device->CreateBuffer( &m_cbufMatrixDesc, &InitData, &cBuffer );

	// Set the buffer.
	m_deviceContext->VSSetConstantBuffers( 0, 1, &cBuffer );
	cBuffer->Release();

	// configure Input Assembler stage
	UINT stride = sizeof(Vertex_PosTex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vBuffer, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_iBuffer, DXGI_FORMAT_R16_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// set wireframe (or not)
	m_deviceContext->RSSetState(m_wireframe ? m_rastWire : m_rastSolid);

	float x = 0.0f,y = 0.0f;

	if(lmbState)
	{
		POINT mouseCoords;
		GetCursorPos(&mouseCoords);	// get mouse coordinates (x,y)
		ScreenToClient(m_windowHandle, &mouseCoords);
		x = (2.0f * mouseCoords.x) / WINDOW_WIDTH - 1.0f;	// Normalise them
		y = 1.0f - (2.0f * mouseCoords.y) / WINDOW_HEIGHT;
		D3DXVECTOR3* origin = new D3DXVECTOR3(x,y,0.0f);
		D3DXVECTOR3* end = new D3DXVECTOR3(x,y,1.0f);

		D3DXMATRIX *inverseviewprojmx = new D3DXMATRIX();
		D3DXMatrixInverse(inverseviewprojmx,NULL,&(mConstData.viewProjMatrix));
		D3DXVec3TransformCoord(origin,origin,inverseviewprojmx);
		D3DXVec3TransformCoord(end,end,inverseviewprojmx);

		m_terr->checkPoints(origin,end,shiftStatus);

		delete origin;
		delete end;
		delete inverseviewprojmx;
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
	oss << "Camera:\tPOS: x = " << cpos.x << " y = " << cpos.y << " z = " << cpos.z
		<< ";\tROT: x = " << crot.x << " y = " << crot.y << " z = " << crot.z << std:: endl;
	oss << "F1 - wireframe\tF2 - save\t\tF3 - load\t\tF4 - reset" << std::endl;
	oss << "F5 - base terrain\tF6 - Fault Formation\t\tF7 - Improved Perlin Noise\tF8 - Diamond-Square" << std::endl; 
	oss << "F9 - experiment\t";//Selected vertices: ";
	//m_terr->drawSelectedId(oss);

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
void Renderer::moveVertex(float value)
{
	m_terr->vertexUp(value);
	m_terr->refreshVBuffer(&m_vBuffer);
}

void Renderer::shutdown()
{
	if (m_camera)
		delete m_camera;
	m_shader.release();
	m_textures.release();

	if (m_terr)
		m_terr->release();
	if (m_swapChain)
		m_swapChain->Release();
	if (m_renderTargetView)
		m_renderTargetView->Release();
	if (m_backBuffer)
		m_backBuffer->Release();
	if (m_depthStencil)
		m_depthStencil->Release();
	if (m_DSState)
		m_DSState->Release();
	if (m_depthStencilView)
		m_depthStencilView->Release();
	if (m_FW1Factory)
		m_FW1Factory->Release();
	if (m_FontWrapper)
		m_FontWrapper->Release();
	if (m_vBuffer)
		m_vBuffer->Release();
	if (m_iBuffer)
		m_iBuffer->Release();
	//if (m_mBuffer)
	//	m_mBuffer->Release();
	if (m_rastSolid)
		m_rastSolid->Release();
	if (m_rastWire)
		m_rastWire->Release();
	if (m_device)
		m_device->Release();
	if (m_deviceContext)
		m_deviceContext->Release();
}

bool Renderer::saveTerrain(std::string filename)
{
	if (m_terr)
		m_terr->saveToFile(filename);
	else return false;

	return true;
}

bool Renderer::loadTerrain(std::string filename)
{
	if (!m_terr)
		return false;
	m_terr->release();
	m_terr = new Terrain(m_device);
	if (filename.length() > 0)
	{
		if (!m_terr->loadFromFile(filename))
			return false;
	}
	else
		m_terr->reset();
	
	if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
		return false;
	if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
		return false;
	
	m_terr->refreshVBuffer(&m_vBuffer);
	return true;
}

bool Renderer::doExperiment()
{
	int counter = 0;
	std::ifstream file;
	std::string str, str2;
	file.open("test.config");
	if (!file.is_open())
		return false;
	while (file >> str)
	{
		++counter;
		if (str != "==")
			return false;
		file >> str;
		if (str == "PN")	// Perlin noise
		{
			IMP_Params par;
			file >> par.OCTAVES;
			file >> par.FREQ;
			file >> par.DISPLACEMENT;
			// do single experiment
			ImprovedPerlinNoise* ipn_terr = new ImprovedPerlinNoise(m_device);
			ipn_terr->setParameters(par);
			m_terr->release();
			m_terr = ipn_terr;
			if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
				return false;
			if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
				return false;

			// save results
			CreateDirectory(L"test", NULL);
			m_terr->saveToFile("test\\" + tostr(counter) + " " + str + "_" 
									+ tostr(par.OCTAVES) + "_"
									+ tostr(par.FREQ) + "_"
									+ tostr(par.DISPLACEMENT) + ".bmp");
		}
		else if (str == "FF")	// Fault form.
		{
			FF_Params par;
			file >> par.ITERATIONS;
			file >> par.DISPLACEMENT;
			file >> par.WAVE;
			// do single experiment
			FaultForm* ff_terr = new FaultForm(m_device);
			ff_terr->setParameters(par);
			m_terr->release();
			m_terr = ff_terr;
			if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
				return false;
			if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
				return false;

			// save results
			CreateDirectory(L"test", NULL);
			m_terr->saveToFile("test\\" + tostr(counter) + " " + str	+ "_" 
									+ tostr(par.ITERATIONS) + "_"
									+ tostr(par.DISPLACEMENT) + "_"
									+ tostr(par.WAVE) + ".bmp");
		}
		else if (str == "DS")	// Diamond-Square
		{
			// read parameters
			DS_Params par;
			file >> par.ROUGHNESS;
			file >> par.DISPLACEMENT;
			file >> str2;
			if (str2 == "true")
				par.RANDOM_SEEDS = true;
			else
				par.RANDOM_SEEDS = false;
			// do single experiment
			DiamondSquare* ds_terr = new DiamondSquare(m_device);
			ds_terr->setParameters(par);
			m_terr->release();
			m_terr = ds_terr;
			if (!m_terr->createVertices(&m_vBuffer, &m_numberOfVertices))
				return false;
			if (!m_terr->createIndices(&m_iBuffer, &m_numberOfIndices))
				return false;

			// save results
			CreateDirectory(L"test", NULL);
			m_terr->saveToFile("test\\" + tostr(counter) + " " + str	+ "_" 
									+ tostr(par.ROUGHNESS) + "_"
									+ tostr(par.DISPLACEMENT) + "_"
									+ tostr(par.RANDOM_SEEDS) + ".bmp");
		}
		else
			return false;

		// uncomment to see results :)
		D3DXVECTOR3 v = D3DXVECTOR3(0.0, 0.0, 0.0);
		renderFrame(v, v, false, false);
		Sleep(300);
	}
	file.close();
	return true;
}