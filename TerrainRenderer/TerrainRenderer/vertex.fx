struct VIn
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VOut
{
    float4 clipPos : SV_POSITION;
	//float3 worldPos : TEXCOORD8;
	//float3 worldNormal : TEXCOORD9;
	float2 texCoord : TEXCOORD0;
};

cbuffer cb_mx : register(b0)
{
	matrix worldMatrix;
	matrix viewProjectionMatrix;
};


VOut VShader(VIn input)
{
    VOut output;
	float3 worldPos, worldNormal;

	worldPos = mul(worldMatrix, float4(input.position.xyz, 1.0)).xyz;
	output.clipPos = mul(viewProjectionMatrix, float4(worldPos.xyz, 1.0));	
	worldNormal = normalize(mul(worldMatrix, float4( 0, 1, 0, 0)).xyz);		// TUTAJ PRZYDA£ABY SIÊ NORMALNA MODELU ZMIAST 0,1,0

	output.texCoord = input.texCoord;

    return output;
}

