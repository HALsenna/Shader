cbuffer HP : register(b0)
{
	float value;
	float2 uvMovePos;
	float dummy;
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
	// X���W�̈ړ�
	input.uv.x = input.uv.x + uvMovePos.x;
	if (input.uv.x > 1.0f)
	{
		input.uv.x = input.uv.x - 1.0f;
	}
	// Y���W�̈ړ�
	input.uv.y = input.uv.y + uvMovePos.y;
	if (input.uv.y > 1.0f)
	{
		input.uv.y = input.uv.y - 1.0f;
	}
	float4 color = tex.Sample(samp, input.uv);
	clip(value - input.uv.x);
	return color;
}