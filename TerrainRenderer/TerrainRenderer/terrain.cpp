#include "terrain.h"

Terrain::Terrain(ID3D11Device *dev)
	: m_device(dev), rows(160), cols(160)
{
	selectedId.clear();
}

bool Terrain::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	vertices.clear();

	float rowCnt = -TERR_WIDTH,
		colCnt = -TERR_HEIGHT;
	float rowStep = 2.0f * (float)TERR_WIDTH/(float)rows;
	float colStep = 2.0f * (float)TERR_HEIGHT/(float)cols;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (loadedPos.size() > 0) 
				vertices.push_back(Vertex_PosTex(rowCnt, loadedPos[i*rows + j], colCnt, i%2, j%2));
			else
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

/*void Terrain::calculateNormals()
{
	D3DXVECTOR3 va,vb,vc,vd,ve,normal;

	for (int i = 1; i < (rows-1); ++i)
		for (int j = 1; j < (cols-1); ++j)
		{
			va = D3DXVECTOR3(vertices[i*cols + j-1].x,vertices[i*cols + j-1].y,vertices[i*cols + j-1].z);
			vb = D3DXVECTOR3(vertices[(i-1)*cols + j].x,vertices[(i-1)*cols + j].y,vertices[(i-1)*cols + j].z);
			vc = D3DXVECTOR3(vertices[i*cols + j].x,vertices[i*cols + j].y,vertices[i*cols + j].z);
			vd = D3DXVECTOR3(vertices[(i+1)*cols + j].x,vertices[(i+1)*cols + j].y,vertices[(i+1)*cols + j].z);
			ve = D3DXVECTOR3(vertices[i*cols + j+1].x,vertices[i*cols + j+1].y,vertices[i*cols + j+1].z);

			CalculateNormalFromFourTriangles(&va,&vb,&vc,&vd,&ve,&normal);
			vertices[i*cols + j].a = normal.x;
			vertices[i*cols + j].b = normal.y;
			vertices[i*cols + j].c = normal.z;
		}
}

void Terrain::CalculateNormalFromFourTriangles(D3DXVECTOR3* va, D3DXVECTOR3* vb, D3DXVECTOR3* vc, 
						 D3DXVECTOR3* vd, D3DXVECTOR3* ve, D3DXVECTOR3* normal)
{
		D3DXVECTOR3 VectorCA,VectorCB,VectorCD,VectorCE;
		D3DXVECTOR3 NormalACB,NormalBCE,NormalECD,NormalDCA;
		D3DXVECTOR3 NormalizedACB,NormalizedBCE,NormalizedECD,NormalizedDCA;
		D3DXVECTOR3 AverageNormal;
	
		// Subtract two 3-D vectors ie. v3 = v1 - v2.
		D3DXVec3Subtract(&VectorCA,va,vc);
		D3DXVec3Subtract(&VectorCB,vb,vc);
		D3DXVec3Subtract(&VectorCD,vd,vb);
		D3DXVec3Subtract(&VectorCE,ve,vc);
	
		D3DXVec3Cross(&NormalACB,&VectorCA,&VectorCB);
		D3DXVec3Cross(&NormalBCE,&VectorCB,&VectorCE);
		D3DXVec3Cross(&NormalECD,&VectorCE,&VectorCD);
		D3DXVec3Cross(&NormalDCA,&VectorCD,&VectorCA);
	
		D3DXVec3Normalize(&NormalizedACB, &NormalACB);
		D3DXVec3Normalize(&NormalizedBCE, &NormalBCE);
		D3DXVec3Normalize(&NormalizedECD, &NormalECD);
		D3DXVec3Normalize(&NormalizedACB, &NormalACB);
	
		AverageNormal.x=(NormalizedACB.x + NormalizedBCE.x + NormalizedECD.x + NormalizedACB.x)/(float)4.0;
		AverageNormal.y=(NormalizedACB.y + NormalizedBCE.y + NormalizedECD.y + NormalizedACB.y)/(float)4.0;
		AverageNormal.z=(NormalizedACB.z + NormalizedBCE.z + NormalizedECD.z + NormalizedACB.z)/(float)4.0;
	
		D3DXVec3Normalize(normal, &AverageNormal);
}*/


bool Terrain::refreshVBuffer(ID3D11Buffer **vBuffer)
{
	HRESULT hr;
	// vertex buffer
	// calculateNormals();
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

	for (int i = 0; i < (rows-1); ++i)
		for (int j = 0; j < (cols-1); ++j)
		{
			indices.push_back(i*cols		+ j);
			indices.push_back(i*cols		+ j+1);
			indices.push_back((i+1)*cols	+ j+1);
			
			indices.push_back(i*cols		+ j);
			indices.push_back((i+1)*cols	+ j+1);
			indices.push_back((i+1)*cols	+ j);
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
void Terrain::vertexUp(float value)
{
	for(int i=0;i<selectedId.size();i++)
		vertices[selectedId[i]].y += value;
}
int Terrain::drawSelectedId(std::wstringstream & oss) 
{
	if(selectedId.size() == 0) {
		oss << "NULL";
		return 0;
	}
	for(int i=0;i<selectedId.size();i++)
		oss << selectedId[i] <<",";
	return selectedId.size();
}
bool Terrain::vectorContains(int x)
{
	for(int i=0;i<selectedId.size();i++)
		if(selectedId[i] == x)
			return true;
	return false;
}
int Terrain::checkPoints(D3DXVECTOR3* linep1,D3DXVECTOR3* linep2, bool shiftStatus)
{
	if(!shiftStatus)
		selectedId.clear();

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
			if(!vectorContains(i))
				selectedId.push_back(i);
			return i;
		}
	}
	return -1;
}

bool Terrain::saveToFile(std::string filename)
{
	std::ofstream file;
	file.open(filename);
	if (!file.is_open())
		return false;

	file << rows << "\n";
	file << cols;
	for (int i = 0; i < vertices.size(); ++i)
	{
		file << "\n" << vertices[i].y;
	}

	return true;
}

bool Terrain::loadFromFile(std::string filename)
{
	loadedPos.clear();

	std::ifstream file;
	file.open(filename);
	if (!file.is_open())
		return false;

	file >> rows;
	file >> cols;

	float val;
	while (!file.eof())
	{
		file >> val;
		loadedPos.push_back(val);
	}

	if (loadedPos.size() != rows*cols)
	{
		loadedPos.clear();
		return false;
	}

	return true;
}

void Terrain::reset()
{
	loadedPos.clear();
	for (int i = 0; i < rows*cols; ++i)
		loadedPos.push_back(0.0);
}