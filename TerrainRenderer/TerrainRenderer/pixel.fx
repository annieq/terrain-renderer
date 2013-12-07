struct PIn
{
    float4 clipPos : SV_POSITION;
    float4 color : COLOR;
	float3 worldPos : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;
};


float4 PShader(PIn input) : SV_TARGET
{
    return input.color;
}