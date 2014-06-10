#ifndef PERLIN_H
#define PERLIN_H

#include "terrain.h"

#define FADE(t) ( t * t * t * ( t * ( t * 6 - 15 ) + 10 ) )
#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : ((int)x-1 ) )
#define LERP(t, a, b) ((a) + (t)*((b)-(a)))

using std::vector;

struct IMP_Params
{
	int OCTAVES;		// number of octaves
	int FREQ;			// start frequency
	float DISPLACEMENT; // starting displacement multiplier

	// default parameters' values
	IMP_Params()
	{
		OCTAVES = 3;
		FREQ = 64;
		DISPLACEMENT = 20;
	}
};

class ImprovedPerlinNoise : public Terrain
{
public:
	ImprovedPerlinNoise(ID3D11Device *dev);
	ImprovedPerlinNoise(ID3D11Device *dev, int rows, int cols);
	bool createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices);
	void setParameters(IMP_Params par);

private:
	vector<vector<float>> formTerrain(int rows, int cols, IMP_Params params);
	float grad( int hash, float x, float y );
	float noise( float x, float y );

	IMP_Params m_params;

};


#endif