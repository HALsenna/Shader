struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D rampTex : register(t1);
Texture2D sunViewTex : register(t2);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = modelTex.Sample(samp, pin.uv);

	// --- 影の計算 ---
	// 座標をwで割る
	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;

	// 座標の座標-1~1をテクスチャの座標0~1へ変換
	sunUV = sunUV * 0.5f + 0.5f;
	// 画面のy座標は下から上に向かって増えるが、テクスチャのy座標は上から下に向かって増えるので反転
	sunUV.y = 1.0f - sunUV.y;
	// 奥行情報を書き込んだテクスチャから、距離を取得
	float sunDepth = sunViewTex.Sample(samp, sunUV).r;
	// シャドウアクネ対策
	sunDepth += 0.2f / 100.0f * 0.7f;
	// メインカメラで表示用としているピクセルの太陽から見た距離を取得
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;
	//return float4(pixelDepth, 0.0f, 0.0f, 1.0f);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	float diffuse = dot(N, L);

	// 内積の値を元にランプテクスチャの色を引っ張り出す
	// ※内積の結果は-1～1なので0～1に変換する
	diffuse = diffuse * 0.5f + 0.5f;
	float shadow = rampTex.Sample(samp, float2(diffuse, 0.0f));

	color.rbg *= shadow;
	//color.rgb *= step(pixelDepth, sunDepth) * 0.7f + 0.3f;
	color.rgb *= step(pixelDepth, sunDepth) * 0.7f  + 0.3f;

	return color;
}