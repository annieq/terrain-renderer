#include "terrain.h"

Terrain::Terrain(ID3D11Device *dev)
	: m_device(dev)
{
	selectedId = -1;
}

bool Terrain::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;

	float rowCnt = -TERR_WIDTH,
		colCnt = -TERR_HEIGHT;
	float rowStep = 2.0 * (float)TERR_WIDTH/(float)ROWS;
	float colStep = 2.0 * (float)TERR_HEIGHT/(float)COLS;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			vertices.push_back(Vertex_PosTex(rowCnt, 0.0, colCnt, i%2, j%2));
			colCnt += colStep;
		}
		colCnt = -TERR_HEIGHT;
		rowCnt += rowStep;
	}
	*numOfVertices = vertices.size();

	// vertex buffer
	hr = refreshVBuffer(vBuffer);
	if (FAILED(hr))
		return false;
	
	return true;
}

bool Terrain::refreshVBuffer(ID3D11Buffer **vBuffer)
{
	HRESULT hr;
	// vertex buffer
	D3D11_BUFFER_DESC bufDesc = D3D11_BUFFER_DESC();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof(Vertex_PosTex) * (vertices.size());
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
void Terrain::vertexUp(float value)
{
	if(selectedId >= 0)
		vertices[selectedId].y += value;
}
int Terrain::getSelectedId() 
{
	return selectedId;
}
int Terrain::checkPoints(D3DXVECTOR3* linep1,D3DXVECTOR3* linep2)
{
	float dist = 0.0;
	float length = 0.0f;

	D3DXVECTOR3 *raydir, *vLp1, *cross;
	raydir = new D3DXVECTOR3();
	vLp1 = new D3DXVECTOR3();
	cross = new D3DXVECTOR3();

	raydir->x = linep2->x - linep1->x;
	raydir->y = linep2->y - linep1->y;
	raydir->z = linep2->z - linep1->z;

	D3DXVec3Normalize(raydir,raydir);

	for(int i=0;i<vertices.size();i++)
	{
		vLp1->x = linep1->x - vertices[i].x;	// Wektor z wierzcho³ka do linep1;
		vLp1->y = linep1->y - vertices[i].y;
		vLp1->z = linep1->z - vertices[i].z;
		
		D3DXVec3Cross(cross,vLp1,raydir);
		length = D3DXVec3Length(raydir);
		dist = D3DXVec3Length(cross);
		dist /= length;

		if(dist < 1.0f)
		{
			selectedId = i;
			return i;
		}
	}
	selectedId = -1;
	return -1;
}