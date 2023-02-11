struct PS_IN
{
	// �X�N���[���̍��W-1~1��w�����Ŋ���Ɛ������v�Z�ł���
	// x' = x / w, y' = y / w
	// SV_POSITION�̃p�����[�^�͒��_�V�F�[�_����s�N�Z���V�F�[�_�ɓn�����r���ŏ���Ɍv�Z�ł���
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

// �e�N�X�`��
// register(t0)�̓��f���ɒ���t����f�t�H���g�̃e�N�X�`��
Texture2D modelTex : register(t0);
Texture2D sunViewTex : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	// ���W��w�Ŋ���
	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;
	// ���W�̍��W-1~1���e�N�X�`���̍��W0~1�֕ϊ�
	sunUV = sunUV * 0.5f + 0.5f;
	// ��ʂ�y���W�͉������Ɍ������đ����邪�A�e�N�X�`����y���W�͏ォ�牺�Ɍ������đ�����̂Ŕ��]
	sunUV.y = 1.0f - sunUV.y;

	

	// ���s�����������񂾃e�N�X�`������A�������擾
	float sunDepth = sunViewTex.Sample(samp,sunUV).r;
	// �V���h�E�A�N�l�΍�
	sunDepth += 0.2f / 100.0f * 0.7f ;
	// ���C���J�����ŕ\���p�Ƃ��Ă���s�N�Z���̑��z���猩���������擾
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;

	float4 color = modelTex.Sample(samp, pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	// �g�U��
	float diffuse = dot(N, L);
	// ����
	float ambient = 0.5f;
	float2 playerPos = { 0.5f, 0.5f };
	// �e�̔Z��
	float Darkness = distance(playerPos, );

	// �F�͈̔͂�1~ambient�͈̔͂ɂ���
	color.rgb *= diffuse * 0.5f + ambient;
	// ���݂̃s�N�Z���̉��s�����A�e�N�X�`���ɏ������܂�Ă��鉜�s�����������ꍇ�e�Ɣ��f����
	//���s�N�Z���V�F�[�_�[�ł̓s�N�Z�����ƂɌv�Z����̂ŁA���܂�if���͎g��Ȃ������悢
	color.rgb *= step(pixelDepth, sunDepth) * Darkness + 0.6f;
	//if (pixelDepth > sunDepth)
	//{
	//	color.rgb = 0.0f;
	//}
	//color.rgb *= lerp(sunDepth, pixelDepth, step(pixelDepth, sunDepth));
	//color.rgb *= smoothstep(pixelDepth, sunDepth, 0.5f);

	return color;
}