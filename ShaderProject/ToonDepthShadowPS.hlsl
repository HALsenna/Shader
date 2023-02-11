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

	// --- �e�̌v�Z ---
	// ���W��w�Ŋ���
	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;

	// ���W�̍��W-1~1���e�N�X�`���̍��W0~1�֕ϊ�
	sunUV = sunUV * 0.5f + 0.5f;
	// ��ʂ�y���W�͉������Ɍ������đ����邪�A�e�N�X�`����y���W�͏ォ�牺�Ɍ������đ�����̂Ŕ��]
	sunUV.y = 1.0f - sunUV.y;
	// ���s�����������񂾃e�N�X�`������A�������擾
	float sunDepth = sunViewTex.Sample(samp, sunUV).r;
	// �V���h�E�A�N�l�΍�
	sunDepth += 0.2f / 100.0f * 0.7f;
	// ���C���J�����ŕ\���p�Ƃ��Ă���s�N�Z���̑��z���猩���������擾
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;
	//return float4(pixelDepth, 0.0f, 0.0f, 1.0f);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	float diffuse = dot(N, L);

	// ���ς̒l�����Ƀ����v�e�N�X�`���̐F����������o��
	// �����ς̌��ʂ�-1�`1�Ȃ̂�0�`1�ɕϊ�����
	diffuse = diffuse * 0.5f + 0.5f;
	float shadow = rampTex.Sample(samp, float2(diffuse, 0.0f));

	color.rbg *= shadow;
	//color.rgb *= step(pixelDepth, sunDepth) * 0.7f + 0.3f;
	color.rgb *= step(pixelDepth, sunDepth) * 0.7f  + 0.3f;

	return color;
}