cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
};

// ÉpÉâÉÅÅ[É^
struct VS_INPUT {
	float3	pos	: POSITION;
	float2	uv	: TEXCOORD0;
};

struct VS_OUTPUT {
	float4	pos	: SV_Position;
	float2	uv	: TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = mul(float4(input.pos, 1.0f), g_mWorld);
	output.pos = mul(output.pos, g_mView);
	output.pos = mul(output.pos, g_mProjection);
	output.uv = input.uv;
	return output;
}