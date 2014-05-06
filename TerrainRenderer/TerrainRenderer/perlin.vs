#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : ((int)x-1 ) )

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
	float2 gradient[8];
	int permut[256];
};


float3 fade(float3 t)
{
  return t * t * t * (t * (t * 6 - 15) + 10); // new curve
//  return t * t * (3 - 2 * t); // old curve
}

float fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float perm(float x)
{
  //return tex1D(permSampler, x / 256.0) * 256;
  return permut[x % 256];
}

float grad(float x, float3 p)
{
  //return dot(tex1D(gradSampler, x), p);
  return dot(gradient[x % 8], p);
}

float grad(float x, float2 p)
{
	return dot(gradient[x%8], p);
}

// 3D version
float inoise(float2 p)
{
/*  float2 P = fmod(floor(p), 256.0);
  p -= floor(p);
  float2 f = fade(p);

  // HASH COORDINATES FOR 6 OF THE 8 CUBE CORNERS
  float A = perm(P.x) + P.y;
  //float AA = perm(A) + P.z;
  //float AB = perm(A + 1) + P.z;
  float B =  perm(P.x + 1) + P.y;
  //float BA = perm(B) + P.z;
  //float BB = perm(B + 1) + P.z;

  // AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE

  //return lerp(
  //  lerp(lerp(grad(perm(AA), p),
  //            grad(perm(BA), p + float3(-1, 0, 0)), f.x),
  //       lerp(grad(perm(AB), p + float3(0, -1, 0)),
  //            grad(perm(BB), p + float3(-1, -1, 0)), f.x), f.y),
  //
  //  lerp(lerp(grad(perm(AA + 1), p + float3(0, 0, -1)),
  //            grad(perm(BA + 1), p + float3(-1, 0, -1)), f.x),
  //       lerp(grad(perm(AB + 1), p + float3(0, -1, -1)),
  //            grad(perm(BB + 1), p + float3(-1, -1, -1)), f.x), f.y),
  //
  //  f.z);
  return lerp(
	lerp(grad(perm(A), p),
		 grad
*/
    int ix0, iy0, ix1, iy1;
    float fx0, fy0, fx1, fy1;
    float s, t, nx0, nx1, n0, n1;

    ix0 = FASTFLOOR( p.x ); // Integer part of x
    iy0 = FASTFLOOR( p.y ); // Integer part of y
    fx0 = p.x - ix0;        // Fractional part of x
    fy0 = p.y - iy0;        // Fractional part of y
    fx1 = fx0 - 1.0f;
    fy1 = fy0 - 1.0f;
    ix1 = (ix0 + 1) & 0xff;  // Wrap to 0..255
    iy1 = (iy0 + 1) & 0xff;
    ix0 = ix0 & 0xff;
    iy0 = iy0 & 0xff;
    
    t = fade( fy0 );
    s = fade( fx0 );

    nx0 = grad(permut[ix0 + permut[iy0]], float2(fx0, fy0));
    nx1 = grad(permut[ix0 + permut[iy1]], float2(fx0, fy1));
    n0 = lerp( t, nx0, nx1 );

    nx0 = grad(permut[ix1 + permut[iy0]], float2(fx1, fy0));
    nx1 = grad(permut[ix1 + permut[iy1]], float2(fx1, fy1));
    n1 = lerp(t, nx0, nx1);

    return ( lerp( s, n0, n1 ) );
}

VOut VShader(VIn input)
{
    VOut output;
	input.position.y = inoise(input.position.xz);
	output.worldPos = mul(worldMatrix, float4(input.position.xyz, 1.0)).xyz;
	output.clipPos = mul(viewProjectionMatrix, float4(output.worldPos.xyz, 1.0));	
	output.worldNormal = mul(float3(0,1,0),worldMatrix);

	output.texCoord = input.texCoord;

    return output;
}