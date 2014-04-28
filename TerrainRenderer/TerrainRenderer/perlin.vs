struct VIn
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VOut
{
    float4 clipPos : SV_POSITION;
	float3 worldPos : TEXCOORD8;
	float3 worldNormal : TEXCOORD9;
	float2 texCoord : TEXCOORD0;
};

cbuffer cb_mx : register(b0)
{
	matrix worldMatrix;
	matrix viewProjectionMatrix;
};

cbuffer cb_perlin : register(b1)
{
	int perm[256];
	float3 grad[16];
};


VOut VShader(VIn input)
{
    VOut output;

	output.worldPos = mul(worldMatrix, float4(input.position.xyz, 1.0)).xyz;
	output.clipPos = mul(viewProjectionMatrix, float4(output.worldPos.xyz, 1.0));	
	output.worldNormal = mul(float3(0,1,0),worldMatrix);

	output.texCoord = input.texCoord;

    return output;
}

