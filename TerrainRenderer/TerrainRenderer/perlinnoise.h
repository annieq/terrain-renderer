#ifndef PERLIN_H
#define PERLIN_H

#include "terrain.h"

using std::vector;

struct IMP_Params;

class ImprovedPerlinNoise : public Terrain
{
public:
	ImprovedPerlinNoise(ID3D11Device *dev);
	ImprovedPerlinNoise(ID3D11Device *dev, int rows, int cols);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);

private:
	vector<vector<float>> formTerrain(int rows, int cols, IMP_Params params);
};

struct IMP_Params
{
	//int OCTAVES;

	// default parameters' values
	IMP_Params()
	{
	}
};

#endif