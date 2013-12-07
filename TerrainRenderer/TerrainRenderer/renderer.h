#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "shader.h"
#include "terrain.h"
#include "camera.h"

/// A renderer class (using DX)
class Renderer
{
public:
	Renderer();
	bool initDX(HWND);
	void renderFrame(D3DXVECTOR3,D3DXVECTOR3);
	void shutdown();
	/*
	MO¯E BYC NIEPOTRZEBNE - U¯YWAMY TO W TEJ SAMEJ KLASIE
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	*/

private:
	//bool drawFigure();

private:
	Camera *m_Camera;

	IDXGISwapChain *m_swapChain;
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	ID3D11Texture2D *m_backBuffer;
	ID3D11RenderTargetView *m_renderTargetView;
	D3D11_VIEWPORT m_viewport;
	ID3D11Buffer *m_vBuffer, *m_iBuffer, *m_mBuffer;
	
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;

	unsigned int m_numberOfVertices, m_numberOfIndices;

	Shader shader;
};

struct VERTEX
{
      FLOAT X, Y, Z;      // position
      D3DXCOLOR Color;    // color
};


typedef struct VS_CONSTANT_BUFFER
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewProjMatrix;
} VS_CONSTANT_BUFFER;

#endif