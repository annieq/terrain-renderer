#include "faultformation.h"

FaultForm::FaultForm(ID3D11Device *dev)
	: Terrain(dev)
{}

bool FaultForm::createVertices(ID3D11Buffer **vBuffer, unsigned int *numOfVertices)
{
	return false;
}