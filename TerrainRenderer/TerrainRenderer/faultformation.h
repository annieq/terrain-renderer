#ifndef FAULT_H
#define FAULT_H

#include "terrain.h"

class FaultForm : Terrain
{
public:
	FaultForm(ID3D11Device *dev);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);
};

#endif