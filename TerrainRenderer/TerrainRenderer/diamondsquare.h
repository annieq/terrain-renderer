#ifndef DIAMOND_H
#define DIAMOND_H

#include "terrain.h"

using std::vector;

struct DS_Params
{
	float ROUGHNESS;	// roughness
	float DISPLACEMENT; // starting displacement multiplier
	bool RANDOM_SEEDS; // random init. values

	// default parameters' values
	DS_Params()
	{
		ROUGHNESS = 0.8f;
		DISPLACEMENT = 200;
		RANDOM_SEEDS = false;
	}
};

class DiamondSquare : public Terrain
{
public:
	DiamondSquare(ID3D11Device *dev);
	DiamondSquare(ID3D11Device *dev, int rows, int cols);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);
	void setParameters(DS_Params par);

private:
	vector<vector<float>> formTerrain(int rows, int cols);

	DS_Params m_params;

};


#endif