struct VS_IN
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos:TEXCOORD1;
};
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};


VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	// ローカル座標 > ワールド座標 > ビュー座標 > プロジェクション座標へと変換をかける
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;		// ピクセルシェーダーにワールド座標を渡すため、一旦コピー
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);


	vout.normal = mul(vin.normal, (float3x3)world);
	vout.uv = vin.uv;
	return vout;
}