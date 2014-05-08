#ifndef PERLIN_H
#define PERLIN_H

#include "terrain.h"

#define FADE(t) ( t * t * t * ( t * ( t * 6 - 15 ) + 10 ) )
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : ((int)x-1 ) )
#define LERP(t, a, b) ((a) + (t)*((b)-(a)))

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
	float grad( int hash, float x, float y );
	float noise( float x, float y );

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