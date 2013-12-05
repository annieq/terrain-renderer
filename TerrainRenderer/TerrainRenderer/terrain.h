#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"
#include "shader.h"

class Terrain
{
public:
	Terrain(ID3D11Device *dev);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);
	bool createIndices(ID3D11Buffer **iBuffer, unsigned int *numOfIndices);
	void vertexUp(int vertexX, int vertexZ, float value);	// change the Y-position of vertex (value can be <0)

private:
	ID3D11Device *m_device;
};

#endif