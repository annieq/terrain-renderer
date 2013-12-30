#ifndef SHADER_H
#define	SHADER_H

#include "common.h"

// A class for handling shaders
class Shader
{
public:
	Shader();
	void init(ID3D11Device *dev, ID3D11DeviceContext *devContext, std::vector<ID3D11ShaderResourceView*> texture, ID3D11SamplerState *sampleState);
	void release();

private:
	ID3D11VertexShader *m_vShader;
	ID3D11GeometryShader *m_gShader;
	ID3D11PixelShader *m_pShader;
	ID3D11InputLayout *m_inputLayout;
};


// old vertex type (only color)
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

// vertex type with texture
struct Vertex_PosTex
{
	float x, y, z;
	float u, v;

	Vertex_PosTex() 
		: x(0), y(0), z(0), u(0), v(0)
	{
	}

	Vertex_PosTex(float xx, float yy, float zz)
		: x(xx), y(yy), z(zz), u(0), v(0)
	{
	}

	Vertex_PosTex(float xx, float yy, float zz, float uu, float vv) 
		: x(xx), y(yy), z(zz), u(uu), v(vv)
	{
	}
};


#endif