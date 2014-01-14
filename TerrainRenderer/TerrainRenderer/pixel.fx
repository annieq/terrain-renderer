Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
SamplerState basicSampler;

struct PIn
{
    float4 clipPos : SV_POSITION;
	float3 worldPos : TEXCOORD8;
	float3 worldNormal : TEXCOORD9;
	float2 texCoord : TEXCOORD0;
};


float4 PShader(PIn input) : SV_TARGET
{
	float4 texColor;

	if (input.worldPos.y > 2.0)
	{
		texColor = clamp(input.worldPos.y		   * 0.05, 0, 1) * tex1.Sample(basicSampler, input.texCoord)
				 + clamp((20.0 - input.worldPos.y) * 0.05, 0, 1) * tex0.Sample(basicSampler, input.texCoord);
	}
	else
		texColor = tex0.Sample(basicSampler, input.texCoord);
	return texColor;
}