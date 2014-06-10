#include "diamondsquare.h"

DiamondSquare::DiamondSquare(ID3D11Device *dev)
	: Terrain(dev)
{}

DiamondSquare::DiamondSquare(ID3D11Device *dev, int rows, int cols)
	: Terrain(dev, rows, cols)
{}

bool DiamondSquare::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	vertices.clear();

	float rowCnt = -TERR_WIDTH,
		colCnt = -TERR_HEIGHT;
	float rowStep = 2.0f * (float)TERR_WIDTH/(float)rows;
	float colStep = 2.0f * (float)TERR_HEIGHT/(float)cols;

	//DS_Params params;
	//params.DISPLACEMENT = 512;
	//params.ROUGHNESS = 0.8;
	vector<vector<float>> heights = formTerrain(rows, cols);
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

vector<vector<float>> DiamondSquare::formTerrain(int rows, int cols)
{
	int areaSize = 1;
	vector<vector<float>> heights;
	float disp = m_params.DISPLACEMENT;
	int halfSide;

	// area must be a square with side 2^n - 1
	while ( (areaSize + 1) < rows || (areaSize + 1) < cols)
		areaSize *= 2;
	areaSize += 1;
	
	// make place for values
	heights.resize(areaSize);
	for (int i=0; i<areaSize; ++i)
		heights[i].resize(areaSize);

	srand(time(NULL));

	// initial values
	if (m_params.RANDOM_SEEDS)
	{
		heights[0		  ][0		  ] = -disp + rand() % 2*(int)disp;
		heights[areaSize-1][0		  ] = -disp + rand() % 2*(int)disp;
		heights[0		  ][areaSize-1] = -disp + rand() % 2*(int)disp;
		heights[areaSize-1][areaSize-1] = -disp + rand() % 2*(int)disp;
	}
	else
		heights[0][0] = heights[0][areaSize-1] 
					  = heights[areaSize-1][0] = heights[areaSize-1][areaSize-1] 
					  = 0.0;

	for (int side = areaSize-1; side >= 2; side /= 2)
	{
		halfSide = side/2;

		// square step
		for (int x = 0; x < areaSize-1; x += side)
		{
			for (int y = 0; y < areaSize-1; y += side)
			{
				float avg = heights[x][y]
						  + heights[x+side][y]
						  + heights[x][y+side]
						  + heights[x+side][y+side];
				avg /= 4.0;
				float random = rand() % (int)ceil(2*disp) - disp;	// from (-disp; disp)

				heights[x+halfSide][y+halfSide] = avg + random * m_params.ROUGHNESS;
			}
		}

		// diamond step
		// x,y is a center of a diamond
		for (int x = 0; x < areaSize-1; x += halfSide)
		{
			for (int y = (x+halfSide)%side; y < areaSize-1; y += side)
			{
				float avg;
				if (x == 0)
				{
					avg = heights[(x+halfSide)%areaSize][y]
						  + heights[x][(y+halfSide)			  %areaSize]
						  + heights[x][(y-halfSide + areaSize)%areaSize];
					avg /= 3.0;
				}
				else if (x == areaSize-1 - halfSide)
				{
					avg = heights[(x-halfSide + areaSize)	   %areaSize][y]
							  + heights[x][(y+halfSide)			  %areaSize]
							  + heights[x][(y-halfSide + areaSize)%areaSize];
					avg /= 3.0;
				}
				else if (y == 0)
				{
					avg = heights[(x-halfSide + areaSize)	   %areaSize][y]
							  + heights[(x+halfSide)		   %areaSize][y]
							  + heights[x][(y+halfSide)			  %areaSize];
					avg /= 3.0;
				}
				else if (y == areaSize-1 - side)
				{
					avg = heights[(x-halfSide + areaSize)	   %areaSize][y]
							  + heights[(x+halfSide)		   %areaSize][y]
							  + heights[x][(y-halfSide + areaSize)%areaSize];
					avg /= 3.0;
				}
				else
				{
					avg = heights[(x-halfSide + areaSize)	   %areaSize][y]
						  + heights[(x+halfSide)		   %areaSize][y]
						  + heights[x][(y+halfSide)			  %areaSize]
						  + heights[x][(y-halfSide + areaSize)%areaSize];
					avg /= 4.0;
				}
				float random = rand() % (int)ceil(2*disp) - disp;	// from (-disp; disp)

				heights[x][y] = avg + random * m_params.ROUGHNESS;
				
				if (x == 0) heights[areaSize-1][y] = avg;
				if (y == 0) heights[x][areaSize-1] = avg;
			}
		}

		disp /= 2.0;
	}

	// trim array
	heights.resize(cols);
	for (int i=0; i<cols; ++i)
		heights[i].resize(rows);

	return heights;
}

void DiamondSquare::setParameters(DS_Params par)
{
	m_params.DISPLACEMENT = par.DISPLACEMENT;
	m_params.RANDOM_SEEDS = par.RANDOM_SEEDS;
	m_params.ROUGHNESS = par.ROUGHNESS;
}