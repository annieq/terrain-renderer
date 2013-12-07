struct VOut
{
    float4 clipPos : SV_POSITION;
    float4 color : COLOR;
	float3 worldPos : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;
};

cbuffer cb_mx : register(b0)
{
	matrix worldMatrix;
	matrix viewProjectionMatrix;
};


VOut VShader(float4 position : POSITION, float4 color: COLOR)
{
    VOut output;

    output.color = color;
	output.worldPos = mul(worldMatrix, float4(position.xyz, 1.0)).xyz;
	output.clipPos = mul(viewProjectionMatrix, float4(output.worldPos.xyz, 1.0));	
	output.worldNormal = normalize(mul(worldMatrix, float4( 0, 1, 0, 0)).xyz);		// TUTAJ PRZYDA£ABY SIÊ NORMALNA MODELU ZMIAST 0,1,0

    return output;
}

