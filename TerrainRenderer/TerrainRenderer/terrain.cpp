#include "terrain.h"

Terrain::Terrain(ID3D11Device *dev)
	: m_device(dev)
{
}

bool Terrain::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	std::vector<Vertex_PosCol> vertices;

	float rowCnt = -1.0, colCnt = -1.0;
	float rowStep = 2.0/(float)ROWS;
	float colStep = 2.0/(float)COLS;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			vertices.push_back(Vertex_PosCol(rowCnt, -0.5, colCnt, D3DXCOLOR(i%2,i%2,j%2,1.0) ));
			colCnt += colStep;
		}
		colCnt = -1.0;
		rowCnt += rowStep;
	}
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

	for (int i = 0; i < (ROWS-1); ++i)
		for (int j = 0; j < (COLS-1); ++j)
		{
			indices.push_back(i*COLS		+ j);
			indices.push_back(i*COLS		+ j+1);
			indices.push_back((i+1)*COLS	+ j+1);
			
			indices.push_back(i*COLS		+ j);
			indices.push_back((i+1)*COLS	+ j+1);
			indices.push_back((i+1)*COLS	+ j);
		}
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