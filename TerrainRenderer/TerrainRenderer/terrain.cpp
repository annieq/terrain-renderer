#include "terrain.h"

Terrain::Terrain(ID3D11Device *dev)
	: m_device(dev)
{
}

bool Terrain::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	std::vector<Vertex_PosTex> vertices;

	float rowCnt = -TERR_WIDTH,
		colCnt = -TERR_HEIGHT;
	float rowStep = 2.0 * (float)TERR_WIDTH/(float)ROWS;
	float colStep = 2.0 * (float)TERR_HEIGHT/(float)COLS;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			//vertices.push_back(Vertex_PosCol(rowCnt, -0.5, colCnt, D3DXCOLOR(i%2,i%2,j%2,1.0) ));
			vertices.push_back(Vertex_PosTex(rowCnt, -0.5, colCnt, i%2, j%2));
			colCnt += colStep;
		}
		colCnt = -TERR_HEIGHT;
		rowCnt += rowStep;
	}
	//vertices.push_back(Vertex_PosTex(0, -0.5, 0, 0.0, 1.0));
	//vertices.push_back(Vertex_PosTex(0, -0.5, TERR_HEIGHT, 1.0, 1.0));
	//vertices.push_back(Vertex_PosTex(TERR_WIDTH, -0.5, TERR_HEIGHT, 1.0, 0.0));
	//vertices.push_back(Vertex_PosTex(TERR_WIDTH, -0.5, 0, 0.0, 0.0));
	*numOfVertices = vertices.size();

	// vertex buffer
	D3D11_BUFFER_DESC bufDesc = D3D11_BUFFER_DESC();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof(Vertex_PosTex) * (*numOfVertices);
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
	//indices.push_back(0);
	//indices.push_back(1);
	//indices.push_back(2);
	//indices.push_back(2);
	//indices.push_back(3);
	//indices.push_back(0);
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