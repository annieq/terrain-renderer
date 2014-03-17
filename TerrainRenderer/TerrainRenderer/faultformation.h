#ifndef FAULT_H
#define FAULT_H

#include "terrain.h"
#include <cstdlib>
#include <ctime>

using std::vector;

class FaultForm : Terrain
{
public:
	FaultForm(ID3D11Device *dev);
	FaultForm(ID3D11Device *dev, int rows, int cols);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);

private:
	vector<vector<float>> formTerrain(int rows, int cols, int iter=1000);
};

#endif