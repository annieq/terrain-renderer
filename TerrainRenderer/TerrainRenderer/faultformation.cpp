#include "faultformation.h"

FaultForm::FaultForm(ID3D11Device *dev)
	: Terrain(dev)
{}

FaultForm::FaultForm(ID3D11Device *dev, int rows, int cols)
	: Terrain(dev, rows, cols)
{}

bool FaultForm::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	HRESULT hr;
	vertices.clear();

	float rowCnt = -TERR_WIDTH,
		colCnt = -TERR_HEIGHT;
	float rowStep = 2.0f * (float)TERR_WIDTH/(float)rows;
	float colStep = 2.0f * (float)TERR_HEIGHT/(float)cols;

	vector<vector<float>> heights = formTerrain(rows, cols, 10);
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

vector<vector<float>> FaultForm::formTerrain(int rows, int cols, int iter)
{
	int x1, x2, y1, y2; // 2 points
	float a, b, c;		// line
	float disp = 15.0;
	float step = disp/(float)iter;
	float wave = 250.0;
	float dist;

	// 2D array for heights
	vector<vector<float>> heights;
	heights.resize(cols);
	for (int i=0; i<cols; ++i)
		heights[i].resize(rows);
	// initialize with 0
	for (int i=0; i<cols; ++i)
		for (int j=0; j<rows; ++j)
			heights[i][j] = 0.0;	// heights[height][width]

	srand(time(NULL));

	// main loop
	for (int it=0; it<iter; ++it)
	{
		// get 2 random points
		x1 = rand() % rows; y1 = rand() % cols;
		x2 = rand() % rows; y2 = rand() % cols;
		// calculate the line equation
		a = y2 - y1;
		b = -(x2 - x1);
		c = - x1*(y2 - y1) + y1*(x2 - x1);

		// calculate heights
		for (int i=0; i<cols; ++i)
			for (int j=0; j<rows; ++j)
			{
				//if (a*j + b*i + c > 0)
				//	heights[i][j] += disp;
				//else
				//	heights[i][j] -= disp;

				// sine function
				dist = a*j + b*i + c;
				if (dist > wave)
					heights[i][j] += disp;
				else if (dist < -wave)
					heights[i][j] -= disp;
				else
					//heights[i][j] += sin(dist * 3.14) * disp/2;
					heights[i][j] += disp * sin(1.0/(float)(2*wave) * PI * dist);// - 0.2*wave*PI);
					//heights[i][j] += -disp * cos(1.0/(float)(2*wave) * dist * PI + wave);
			}
		//disp -= step;
	}
	return heights;
}