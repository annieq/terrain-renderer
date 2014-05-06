#include "shader.h"

Shader::Shader()
	: m_device(NULL), m_deviceContext(NULL),
	  m_vShader(NULL), m_gShader(NULL), m_pShader(NULL), m_inputLayout(NULL)
{
}

void Shader::init(ID3D11Device *dev, ID3D11DeviceContext *devContext, std::vector<ID3D11ShaderResourceView*> texture, ID3D11SamplerState *sampleState)
{
	m_device = dev;
	m_deviceContext = devContext;

	ID3D10Blob *vsBlob, *psBlob, *gsBlob;
	D3DX11CompileFromFile(L"../TerrainRenderer/base.vs", NULL, NULL, "VShader", "vs_5_0", NULL, NULL, NULL, &vsBlob, NULL, NULL);
	D3DX11CompileFromFile(L"../TerrainRenderer/base.gs", NULL, NULL, "GShader", "gs_5_0", NULL, NULL, NULL, &gsBlob, NULL, NULL);
	D3DX11CompileFromFile(L"../TerrainRenderer/base.ps", NULL, NULL, "PShader", "ps_5_0", NULL, NULL, NULL, &psBlob, NULL, NULL);

	// vertex shader
	dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vShader);
	devContext->VSSetShader(m_vShader, NULL, 0);

	// geometry shader
	dev->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &m_gShader);
	devContext->GSSetShader(m_gShader, NULL, 0);

	// pixel shader
	dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pShader);
	devContext->PSSetShader(m_pShader, NULL, 0);
	for (int i = 0; i < texture.size(); ++i)
		devContext->PSSetShaderResources(i, 1, &texture[i]);
	devContext->PSSetSamplers(0, 1, &sampleState);

	// input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> vBufElements;
	D3D11_INPUT_ELEMENT_DESC clipPos =	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0};
	D3D11_INPUT_ELEMENT_DESC tex =		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	vBufElements.push_back(clipPos);
	vBufElements.push_back(tex);
	dev->CreateInputLayout(&vBufElements[0], vBufElements.size(), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout);
	devContext->IASetInputLayout(m_inputLayout);
}

void Shader::updateShader(ShaderType type, LPCWSTR filename)
{
	ID3D10Blob *shaderBlob, *errorMsg;
	char *errors;
	switch (type)
	{
	case VSHADER:
		D3DX11CompileFromFile(filename, NULL, NULL, "VShader", "vs_5_0", NULL, NULL, NULL, &shaderBlob, &errorMsg, NULL);
		errors = (char*)(errorMsg->GetBufferPointer());
		m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &m_vShader);
		m_deviceContext->VSSetShader(m_vShader, NULL, 0);
		break;
	case GSHADER:
		D3DX11CompileFromFile(filename, NULL, NULL, "GShader", "gs_5_0", NULL, NULL, NULL, &shaderBlob, NULL, NULL);
		m_device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &m_gShader);
		m_deviceContext->GSSetShader(m_gShader, NULL, 0);
		break;
	case PSHADER:
		D3DX11CompileFromFile(filename, NULL, NULL, "PShader", "ps_5_0", NULL, NULL, NULL, &shaderBlob, &errorMsg, NULL);
		errors = (char*)(errorMsg->GetBufferPointer());
		m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &m_pShader);
		m_deviceContext->PSSetShader(m_pShader, NULL, 0);
		break;
	}
}

void Shader::release()
{
	if (m_vShader)
		m_vShader->Release();
	if (m_gShader)
		m_gShader->Release();
	if (m_pShader)
		m_pShader->Release();
	if (m_inputLayout)
		m_inputLayout->Release();
}