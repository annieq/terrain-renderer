#include "common.h"

// A class for handling textures
class Textures
{
public:
	Textures();
	bool init(ID3D11Device *dev, ID3D11DeviceContext *devContext);
	void release();

	std::vector<ID3D11ShaderResourceView*> getTextures();
	ID3D11SamplerState* getSamplerState();

private:
	std::vector<ID3D11ShaderResourceView*> m_textures;
	ID3D11SamplerState *m_samplerState;
};