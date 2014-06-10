#ifndef FAULT_H
#define FAULT_H

#include "terrain.h"
#include <cstdlib>
#include <ctime>

using std::vector;

struct FF_Params;

class FaultForm : public Terrain
{
public:
	FaultForm(ID3D11Device *dev);
	FaultForm(ID3D11Device *dev, int rows, int cols);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);

private:
	vector<vector<float>> formTerrain(int rows, int cols, FF_Params params);
};

struct FF_Params
{
	int ITERATIONS;		// number of iterations
	float DISPLACEMENT;	// initial displacement
	float WAVE;			// wave length

	// default parameters' values
	FF_Params()
	{
		ITERATIONS = 500;
		DISPLACEMENT = 8.0;
		WAVE = 750;
	}
};

#endif