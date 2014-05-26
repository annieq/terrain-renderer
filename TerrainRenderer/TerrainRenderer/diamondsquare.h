#ifndef DIAMOND_H
#define DIAMOND_H

#include "terrain.h"

using std::vector;

struct DS_Params;

class DiamondSquare : public Terrain
{
public:
	DiamondSquare(ID3D11Device *dev);
	DiamondSquare(ID3D11Device *dev, int rows, int cols);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);

private:
	vector<vector<float>> formTerrain(int rows, int cols, DS_Params params);

};

struct DS_Params
{
	//int OCTAVES;		// number of octaves
	//int FREQ;			// start frequency
	//float DISPLACEMENT; // starting displacement multiplier

	// default parameters' values
	DS_Params()
	{
		//OCTAVES = 3;
		//FREQ = 64;
		//DISPLACEMENT = 20;
	}
};

#endif