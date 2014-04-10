#include "terrain.h"

Terrain::Terrain(ID3D11Device *dev)
	: m_device(dev), rows(256), cols(256)
{
	selectedId.clear();
}

Terrain::Terrain(ID3D11Device *dev, int rows, int cols)
	: m_device(dev), rows(rows), cols(cols)
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
	for (int j = 0; j < cols; ++j)
	{
		for (int i = 0; i < rows; ++i)
		{
			if (loadedPos.size() > 0) 
				vertices.push_back(Vertex_PosTex(rowCnt, loadedPos[j*rows + i], colCnt, i%2, j%2));
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
		vertices[selectedId[i]].y += (vertices[selectedId[i]].y > 64.0 || vertices[selectedId[i]].y < -64.0) ? 0 : value;
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

			delete raydir;
			delete vLp1;
			delete cross;
			return i;
		}
	}
	delete raydir;
	delete vLp1;
	delete cross;
	return -1;
}

bool Terrain::saveToFile(std::string filename)
{
	double value;
	cimg_library::CImg<> img(rows, cols, 1, 3);
	for (int j = 0; j < cols; ++j)
		for (int i = 0; i < rows; ++i)
		{
			//value = 255.0/128.0 * (vertices[i + j*rows].y + 64.0);
			value = vertices[i + j*rows].y + 128.0;
			if (value > 255.0)
				value = 255.0;
			else if (value < 0)
				value = 0.0;
			img.atXY(i, j, 0, 0) = value;
			img.atXY(i, j, 0, 1) = 0;
			img.atXY(i, j, 0, 2) = 0;
		}
	img.channel(0);
	img.save(filename.c_str());

	return true;
}

bool Terrain::loadFromFile(std::string filename)
{
	loadedPos.clear();

	cimg_library::CImg<> img(filename.c_str());
	img.channel(0);
	float pix = 0.0;
	for (int i = 0; i < img.size(); ++i)
	{
		pix = img.at(i);
		//loadedPos.push_back( 128.0/255.0 * pix - 64.0 );
		loadedPos.push_back(pix - 128.0);
	}
	rows = img.width();
	cols = img.height();

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

void Terrain::release()
{
	if (m_device)
		m_device->Release();
}