struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer DirectionalLight : register(b0)
{
	float3 lightDir;
	float dummy;
	float4 lightColor;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	// �e�N�X�`���̃T���v�����O
	float4 texColor = tex.Sample(samp, pin.uv);
	float3 N = normalize(pin.normal);		// �ʂ̖@��
	float3 L = normalize(lightDir);			// ��������ʂւ̃x�N�g��

	float diffuse = dot(N, L);
	float Ambient = 0.3f;
	texColor.xyz *= diffuse * Ambient + Ambient;
	return texColor;
}