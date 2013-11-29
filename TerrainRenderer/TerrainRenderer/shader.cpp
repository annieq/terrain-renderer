#include "shader.h"

void Shader::init(ID3D11Device *dev, ID3D11DeviceContext *devContext)
{
	ID3D10Blob *vsBlob, *psBlob;
	D3DX11CompileFromFile(L"verysimpleshaders.fx", NULL, NULL, "VShader", "vs_1_1", NULL, NULL, NULL, &vsBlob, NULL, NULL);
	D3DX11CompileFromFile(L"verysimpleshaders.fx", NULL, NULL, "PShader", "ps_2_0", NULL, NULL, NULL, &psBlob, NULL, NULL);

	// vertex shader
	dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vShader);
	devContext->VSSetShader(m_vShader, NULL, 0);

	// pixel shader
	dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_pShader);
	devContext->PSSetShader(m_pShader, NULL, 0);
}

void Shader::release()
{
	if (m_vShader)
		m_vShader->Release();
	if (m_pShader)
		m_pShader->Release();
}