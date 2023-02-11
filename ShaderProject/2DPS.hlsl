cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
};
float4 g_color : register(b1);		// 色情報
cbuffer rate : register(b2)			// 演出の割合
{
	float rate;
	float3 dummy;
}

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	uv	: TEXCOORD0;
};

Texture2D    g_texture : register(t0);		// テクスチャ
Texture2D	 g_dissolveTex : register(t1);	// ディゾルブ用のテクスチャ
SamplerState samp : register(s0);			// サンプラ

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = g_texture.Sample(samp, input.uv) * g_color;
	float dissolve = g_dissolveTex.Sample(samp, input.uv).r;
	float show = step(rate, dissolve);
	Color.a *= show;
	return Color;
}