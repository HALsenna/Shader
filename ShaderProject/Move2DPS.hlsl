cbuffer HP : register(b0)
{
	float value;
	float2 uvMovePos;
	float dummy;
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
	// X座標の移動
	input.uv.x = input.uv.x + uvMovePos.x;
	if (input.uv.x > 1.0f)
	{
		input.uv.x = input.uv.x - 1.0f;
	}
	// Y座標の移動
	input.uv.y = input.uv.y + uvMovePos.y;
	if (input.uv.y > 1.0f)
	{
		input.uv.y = input.uv.y - 1.0f;
	}
	float4 color = tex.Sample(samp, input.uv);
	clip(value - input.uv.x);
	return color;
}