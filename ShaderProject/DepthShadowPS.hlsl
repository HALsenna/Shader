struct PS_IN
{
	// スクリーンの座標-1~1はw成分で割ると正しく計算できる
	// x' = x / w, y' = y / w
	// SV_POSITIONのパラメータは頂点シェーダからピクセルシェーダに渡される途中で勝手に計算できる
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

// テクスチャ
// register(t0)はモデルに張り付けるデフォルトのテクスチャ
Texture2D modelTex : register(t0);
Texture2D sunViewTex : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	// 座標をwで割る
	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;
	// 座標の座標-1~1をテクスチャの座標0~1へ変換
	sunUV = sunUV * 0.5f + 0.5f;
	// 画面のy座標は下から上に向かって増えるが、テクスチャのy座標は上から下に向かって増えるので反転
	sunUV.y = 1.0f - sunUV.y;

	

	// 奥行情報を書き込んだテクスチャから、距離を取得
	float sunDepth = sunViewTex.Sample(samp,sunUV).r;
	// シャドウアクネ対策
	sunDepth += 0.2f / 100.0f * 0.7f ;
	// メインカメラで表示用としているピクセルの太陽から見た距離を取得
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;

	float4 color = modelTex.Sample(samp, pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	// 拡散光
	float diffuse = dot(N, L);
	// 環境光
	float ambient = 0.5f;
	float2 playerPos = { 0.5f, 0.5f };
	// 影の濃さ
	float Darkness = distance(playerPos, );

	// 色の範囲を1~ambientの範囲にする
	color.rgb *= diffuse * 0.5f + ambient;
	// 現在のピクセルの奥行きが、テクスチャに書き込まれている奥行きよりも遠い場合影と判断する
	//※ピクセルシェーダーではピクセルごとに計算するので、あまりif文は使わない方がよい
	color.rgb *= step(pixelDepth, sunDepth) * Darkness + 0.6f;
	//if (pixelDepth > sunDepth)
	//{
	//	color.rgb = 0.0f;
	//}
	//color.rgb *= lerp(sunDepth, pixelDepth, step(pixelDepth, sunDepth));
	//color.rgb *= smoothstep(pixelDepth, sunDepth, 0.5f);

	return color;
}