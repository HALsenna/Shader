cbuffer HP : register(b0)
{
	float value;
	float3 dummy;
};

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	uv	: TEXCOORD0;
};

Texture2D    tex : register(t0);		// �e�N�X�`��
SamplerState samp : register(s0);			// �T���v��

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 color = tex.Sample(samp, input.uv);
	clip(value - input.uv.x);
	return color;
}