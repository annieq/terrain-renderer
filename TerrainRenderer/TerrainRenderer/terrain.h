#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"
#include "shader.h"
#include "external\CImg.h"

#include <string>
#include <fstream>

#define TERR_WIDTH 500
#define TERR_HEIGHT 500

class Terrain
{
public:
	Terrain(ID3D11Device *dev);
	Terrain(ID3D11Device *dev, int rows, int cols);
	virtual bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);
	bool refreshVBuffer(ID3D11Buffer **vBuffer);
	bool createIndices(ID3D11Buffer **iBuffer, unsigned int *numOfIndices);

	void vertexUp(float value);	// change the Y-position of vertex (value can be <0)
	int drawSelectedId(std::wstringstream &);

	int checkPoints(D3DXVECTOR3* linep1,D3DXVECTOR3* linep2, bool shiftStatus);
	bool vectorContains(int x);

	bool saveToFile(std::string filename);
	bool loadFromFile(std::string filename);
	void reset();

	void release();

private:
	ID3D11Device *m_device;

protected:
	int rows, cols;	// number of columns & rows
	std::vector<Vertex_PosTex> vertices;	// list of vertices
	std::vector<float> loadedPos;
	std::vector<int> selectedId;	// list of ID od selected vertices
	
};

#endif