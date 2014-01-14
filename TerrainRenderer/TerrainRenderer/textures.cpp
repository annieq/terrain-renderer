#include "textures.h"

Textures::Textures()
	: m_samplerState(NULL)
{
}

bool Textures::init(ID3D11Device *dev, ID3D11DeviceContext *devContext)
{
	HRESULT hr;
	// prepare texture(s)
	D3D11_SAMPLER_DESC samplerDesc = D3D11_SAMPLER_DESC();

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	// Create the texture sampler state.
	hr = dev->CreateSamplerState(&samplerDesc, &m_samplerState);
	if(FAILED(hr))
		return false;

	m_textures.resize(2);
	hr = D3DX11CreateShaderResourceViewFromFile(dev, L"../TerrainRenderer/resources/grass-256x256.png", NULL, NULL, &m_textures[0], NULL);
	hr = D3DX11CreateShaderResourceViewFromFile(dev, L"../TerrainRenderer/resources/rocks-256x256.png", NULL, NULL, &m_textures[1], NULL);
	if (FAILED(hr))
	{
		m_textures.resize(0);
		return false;
	}

	return true;
}

void Textures::release()
{
	if (m_samplerState)
		m_samplerState->Release();
	for (int i = 0; i < m_textures.size(); ++i)
		m_textures[i]->Release();
}

//////////////////////////////////////////////////// TODO: multitex! ////////////////////////////////////////////////
std::vector<ID3D11ShaderResourceView*> Textures::getTextures()
{
	return m_textures;
}

ID3D11SamplerState* Textures::getSamplerState()
{
	return m_samplerState;
}