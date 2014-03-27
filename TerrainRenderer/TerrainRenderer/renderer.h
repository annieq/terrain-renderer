#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "shader.h"
#include "terrain.h"
#include "faultformation.h"
#include "camera.h"
#include "textures.h"

/// A renderer class (using DX)
class Renderer
{
public:
	Renderer();
	bool initDX(HWND);
	void renderFrame(D3DXVECTOR3,D3DXVECTOR3,bool,bool);
	void shutdown();
	void changeWireframe();
	void moveVertex(float value);

	bool saveTerrain(std::string filename);
	bool loadTerrain(std::string filename = "");

private:
	bool createRasterizerState();
	HRESULT createDepthScencilTexture();
	HRESULT createDepthScencilView();
	HRESULT createDepthScencilState();

private:
	Camera *m_camera;
	Terrain *m_terr;

	IFW1Factory *m_FW1Factory;
	IFW1FontWrapper *m_FontWrapper;

	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	ID3D11Texture2D *m_backBuffer;
	ID3D11Texture2D* m_depthStencil;
	ID3D11DepthStencilState * m_DSState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RenderTargetView *m_renderTargetView;
	ID3D11Buffer *m_vBuffer, *m_iBuffer, *m_mBuffer;
	ID3D11RasterizerState *m_rastSolid, *m_rastWire;

	HWND m_windowHandle;
	
	D3D11_VIEWPORT m_viewport;
	Shader m_shader;
	Textures m_textures;

	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;

	unsigned int m_numberOfVertices, m_numberOfIndices;
	bool m_wireframe;

	DWORD m_NextGameTick;	// time for next frame in ms
};

typedef struct VS_CONSTANT_BUFFER
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewProjMatrix;
} VS_CONSTANT_BUFFER;

#endif