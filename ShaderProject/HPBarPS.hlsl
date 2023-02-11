cbuffer HP : register(b0)
{
	float value;
	float3 dummy;
};

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	uv	: TEXCOORD0;
};

Texture2D    tex : register(t0);		// テクスチャ
SamplerState samp : register(s0);			// サンプラ

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 color = tex.Sample(samp, input.uv);
	clip(value - input.uv.x);
	return color;
}