#include "perlinnoise.h"

ImprovedPerlinNoise::ImprovedPerlinNoise(ID3D11Device *dev)
	: Terrain(dev)
{}

ImprovedPerlinNoise::ImprovedPerlinNoise(ID3D11Device *dev, int rows, int cols)
	: Terrain(dev, rows, cols)
{}

bool ImprovedPerlinNoise::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	vertices.clear();

	float rowCnt = -TERR_WIDTH,
		colCnt = -TERR_HEIGHT;
	float rowStep = 2.0f * (float)TERR_WIDTH/(float)rows;
	float colStep = 2.0f * (float)TERR_HEIGHT/(float)cols;

	IMP_Params params;
	//params.ITERATIONS = 10;
	vector<vector<float>> heights = formTerrain(rows, cols, params);
	for (int j = 0; j < cols; ++j)
	{
		for (int i = 0; i < rows; ++i)
		{
			vertices.push_back(Vertex_PosTex(rowCnt, heights[j][i], colCnt, i%2, j%2));
			colCnt += colStep;
		}
		colCnt = -TERR_HEIGHT;
		rowCnt += rowStep;
	}

	*numOfVertices = vertices.size();
	// vertex buffer
	hr = refreshVBuffer(vBuffer);
	if (FAILED(hr))
		return false;
	
	return true;
}

vector<vector<float>> ImprovedPerlinNoise::formTerrain(int rows, int cols, IMP_Params params)
{
	
	// 2D array for heights
	vector<vector<float>> heights;
	heights.resize(cols);
	for (int i=0; i<cols; ++i)
		heights[i].resize(rows);
	// initialize with 0
	for (int i=0; i<cols; ++i)
		for (int j=0; j<rows; ++j)
			heights[i][j] = 0.0;	// heights[height][width]

	// ...

	return heights;
}