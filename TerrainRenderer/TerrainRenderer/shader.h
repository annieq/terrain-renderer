#ifndef SHADER_H
#define	SHADER_H

//#include <D3Dcompiler.h>
#include "common.h"

// A class for handling shaders
class Shader
{
public:
	Shader();
	void init(ID3D11Device *dev, ID3D11DeviceContext *devContext);
	void release();

private:
	ID3D11VertexShader *m_vShader;
	ID3D11GeometryShader *m_gShader;
	ID3D11PixelShader *m_pShader;
	ID3D11InputLayout *m_inputLayout;
};


// vertex type
struct Vertex_PosCol
{
	float x, y, z;
	//float r, g, b, a;
	D3DXCOLOR color;

	Vertex_PosCol() 
		: x(0), y(0), z(0), color(0,0,0,0)
	{
	}

	Vertex_PosCol(float xx, float yy, float zz)
		: x(xx), y(yy), z(zz), color(0,0,0,0)
	{
	}

	Vertex_PosCol(float xx, float yy, float zz, D3DXCOLOR col) 
		: x(xx), y(yy), z(zz), color(col)
	{
	}
};

#endif