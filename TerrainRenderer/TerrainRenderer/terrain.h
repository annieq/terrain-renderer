#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"
#include "shader.h"

#define ROWS 16
#define COLS 16

#define TERR_WIDTH 100
#define TERR_HEIGHT 100

class Terrain
{
public:
	Terrain(ID3D11Device *dev);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);
	bool createIndices(ID3D11Buffer **iBuffer, unsigned int *numOfIndices);
	void vertexUp(int vertexX, int vertexZ, float value);	// change the Y-position of vertex (value can be <0)

	void vertexUp(float value);
	int getSelectedId();

	int checkPoints(D3DXVECTOR4* linep1,D3DXVECTOR4* linep2);
private:
	ID3D11Device *m_device;
	std::vector<Vertex_PosTex> vertices;
	int selectedId;
};

#endif