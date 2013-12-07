struct GIn
{
    float4 clipPos : SV_POSITION;
    float4 color : COLOR;
	float3 worldPos : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;
};

typedef GIn GOut;

////////////////////////////////////////////////////////////////////////////////
// Geometry Shader	(NA RAZIE NIE U¯YWANY)
////////////////////////////////////////////////////////////////////////////////
static float dim = 0.05;					//global variable marked static is _not_ visible to the app. (not a const-buffer/uniform var.)

[maxvertexcount(12)]
void GSMain(triangle GIn input[3], inout TriangleStream<GOut> outputStream)
{	
	for(int i = 0; i < 3; i++)
	{
		GSOutput out1 = input[i]; out1.clipPos.xy += float2(-dim, -dim);
		GSOutput out2 = input[i]; out2.clipPos.xy += float2(-dim, +dim);	
		GSOutput out3 = input[i]; out3.clipPos.xy += float2(+dim, -dim);
		GSOutput out4 = input[i]; out4.clipPos.xy += float2(+dim, +dim);
		
		outputStream.Append(out1);
		outputStream.Append(out2);		
		outputStream.Append(out3);
		outputStream.Append(out4);
		
		outputStream.RestartStrip();
	}
	
}