struct GIn
{
    float4 clipPos : SV_POSITION;
	float3 worldPos : TEXCOORD8;
	float3 worldNormal : TEXCOORD9;
	float2 texCoord : TEXCOORD0;
};


typedef GIn GOut;

////////////////////////////////////////////////////////////////////////////////
// Geometry Shader
////////////////////////////////////////////////////////////////////////////////

[maxvertexcount(12)]
void GShader(triangle GIn input[3], inout TriangleStream<GOut> outputStream)
{	
	GOut output = (GOut)0;

	output.worldNormal = normalize(cross(input[1].worldPos - input[0].worldPos, input[2].worldPos - input[0].worldPos));
	for (int i = 0; i < 3; i++)
	{
		output.clipPos = input[i].clipPos;
		output.worldPos = input[i].worldPos;
		output.texCoord = input[i].texCoord;
		outputStream.Append(output);
	}
	outputStream.RestartStrip();
}