#include "terrain.h"

Terrain::Terrain(ID3D11Device *dev)
	: m_device(dev)
{
}

bool Terrain::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	std::vector<Vertex_PosCol> vertices;

	vertices.push_back(Vertex_PosCol(+0.5, +0.5, 0.0, D3DXCOLOR(1.0, 0.0, 1.0, 1.0) ));
	vertices.push_back(Vertex_PosCol(-0.5, -0.5, 0.0, D3DXCOLOR(1.0, 1.0, 1.0, 1.0) ));
	vertices.push_back(Vertex_PosCol(-0.5, +0.5, 0.0, D3DXCOLOR(1.0, 1.0, 1.0, 1.0) ));
	vertices.push_back(Vertex_PosCol(+0.5, -0.5, 0.0, D3DXCOLOR(0.0, 0.0, 0.0, 1.0) ));
	*numOfVertices = vertices.size();

	// vertex buffer
	D3D11_BUFFER_DESC bufDesc = D3D11_BUFFER_DESC();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof(Vertex_PosCol) * (*numOfVertices);
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA bufData = D3D11_SUBRESOURCE_DATA();
	bufData.pSysMem = &vertices[0];

	hr = m_device->CreateBuffer(&bufDesc, &bufData, &(*vBuffer));
	if (FAILED(hr))
		return false;
	
	return true;
}

bool Terrain::createIndices(ID3D11Buffer **iBuffer, unsigned int *numOfIndices)
{
	HRESULT hr;
	std::vector<WORD> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(1);
	*numOfIndices = indices.size();
	
	// index buffer
	D3D11_BUFFER_DESC bufDesc = D3D11_BUFFER_DESC();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof(WORD) * (*numOfIndices);
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA bufData = D3D11_SUBRESOURCE_DATA();
	bufData.pSysMem = &indices[0];

	hr = m_device->CreateBuffer(&bufDesc, &bufData, &(*iBuffer));
	if (FAILED(hr))
		return false;

	return true;
}

void Terrain::vertexUp(int vertexX, int vertexZ, float value)
{
	// TODO
}