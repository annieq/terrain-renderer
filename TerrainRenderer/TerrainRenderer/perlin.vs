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
	float3 gradient[16];
	int permut[256];
};


float3 fade(float3 t)
{
  return t * t * t * (t * (t * 6 - 15) + 10); // new curve
//  return t * t * (3 - 2 * t); // old curve
}

float perm(float x)
{
  //return tex1D(permSampler, x / 256.0) * 256;
  return permut[x % 256];
}

float grad(float x, float3 p)
{
  //return dot(tex1D(gradSampler, x), p);
  return dot(gradient[x % 16], p);
}

// 3D version
float inoise(float3 p)
{
  float3 P = fmod(floor(p), 256.0);
  p -= floor(p);
  float3 f = fade(p);

  // HASH COORDINATES FOR 6 OF THE 8 CUBE CORNERS
  float A = perm(P.x) + P.y;
  float AA = perm(A) + P.z;
  float AB = perm(A + 1) + P.z;
  float B =  perm(P.x + 1) + P.y;
  float BA = perm(B) + P.z;
  float BB = perm(B + 1) + P.z;

  // AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE

  return lerp(
    lerp(lerp(grad(perm(AA), p),
              grad(perm(BA), p + float3(-1, 0, 0)), f.x),
         lerp(grad(perm(AB), p + float3(0, -1, 0)),
              grad(perm(BB), p + float3(-1, -1, 0)), f.x), f.y),

    lerp(lerp(grad(perm(AA + 1), p + float3(0, 0, -1)),
              grad(perm(BA + 1), p + float3(-1, 0, -1)), f.x),
         lerp(grad(perm(AB + 1), p + float3(0, -1, -1)),
              grad(perm(BB + 1), p + float3(-1, -1, -1)), f.x), f.y),

    f.z);
}

VOut VShader(VIn input)
{
    VOut output;
	input.position.y += 10*inoise(input.position);
	//input.position.y += 0.1 * permut[input.position.x % 256];
	output.worldPos = mul(worldMatrix, float4(input.position.xyz, 1.0)).xyz;
	output.clipPos = mul(viewProjectionMatrix, float4(output.worldPos.xyz, 1.0));	
	output.worldNormal = mul(float3(0,1,0),worldMatrix);

	output.texCoord = input.texCoord;

    return output;
}