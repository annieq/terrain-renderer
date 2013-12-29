Texture2D tex;
SamplerState basicSampler;

struct PIn
{
    float4 clipPos : SV_POSITION;
	//float3 worldPos : TEXCOORD8;
	//float3 worldNormal : TEXCOORD9;
	float2 texCoord : TEXCOORD0;
};


float4 PShader(PIn input) : SV_TARGET
{
	float4 texColor;
	texColor = tex.Sample(basicSampler, input.texCoord);
	return texColor;
}