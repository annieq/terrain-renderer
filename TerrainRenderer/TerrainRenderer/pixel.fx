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
	float4 ambientColor = float4(0.15f, 0.15f, 0.15f, 1.0f);
	float4 diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float3 lightDirection = float3(0.3f,-0.8f,0.3f);
	float4 texColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

	color = ambientColor;

	if (input.worldPos.y > 2.0)
	{
		texColor = clamp(input.worldPos.y		   * 0.05, 0, 1) * tex1.Sample(basicSampler, input.texCoord)
				 + clamp((20.0 - input.worldPos.y) * 0.05, 0, 1) * tex0.Sample(basicSampler, input.texCoord);
	}
	else
		texColor = tex0.Sample(basicSampler, input.texCoord);

	lightDir = -lightDirection;

	lightIntensity = saturate(dot(input.worldNormal, lightDir));

	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
    }
	color = saturate(color);
	color = color * texColor;
	return color;
}