#include "shader.h"

void Shader::init(ID3D11Device *dev, ID3D11DeviceContext *devContext)
{
	ID3D10Blob *vsBlob, *psBlob; //, *gsBlob;
	D3DX11CompileFromFile(L"../TerrainRenderer/vertex.fx", NULL, NULL, "VShader", "vs_5_0", NULL, NULL, NULL, &vsBlob, NULL, NULL);
	//D3DX11CompileFromFile(L"../TerrainRenderer/geometry.fx",	NULL, NULL, "GShader", "gs_5_0", NULL, NULL, NULL, &gsBlob, NULL, NULL);
	D3DX11CompileFromFile(L"../TerrainRenderer/pixel.fx", NULL, NULL, "PShader", "ps_5_0", NULL, NULL, NULL, &psBlob, NULL, NULL);


	// vertex shader
	dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vShader);
	devContext->VSSetShader(m_vShader, NULL, 0);

	// geometry shader
	// dev->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &m_gShader);
	// devContext->GSSetShader(m_gShader, NULL, 0);

	// pixel shader
	dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pShader);
	devContext->PSSetShader(m_pShader, NULL, 0);


	// input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> vBufElements;
	D3D11_INPUT_ELEMENT_DESC clipPos = {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
	D3D11_INPUT_ELEMENT_DESC color = {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	vBufElements.push_back(clipPos);
	vBufElements.push_back(color);
	dev->CreateInputLayout(&vBufElements[0], vBufElements.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
	devContext->IASetInputLayout(m_inputLayout);
}

void Shader::release()
{
	if (m_vShader)
		m_vShader->Release();
	// if (m_gShader)
	//	m_gShader->Release();
	if (m_pShader)
		m_pShader->Release();
}