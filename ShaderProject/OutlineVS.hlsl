struct VS_IN
{
    float3 pos      : POSITION0;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
};
struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 wPos     : TEXCOORD1;
};
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};
cbuffer Line : register(b1)
{
    float thick;       // ���̑���
    float3 dummy;
};


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);

    // �@���̕����Ɍ������ă��f���̒��_���ړ�������
    vout.pos.xyz += normalize(vin.normal) * thick;

    // ���[�J�����W > ���[���h���W > �r���[���W > �v���W�F�N�V�������W�ƕϊ���������
    vout.pos = mul(vout.pos, world);

    // �s�N�Z���V�F�[�_�[�Ƀ��[���h���W��n�����߃R�s�[
    vout.wPos = vout.pos;
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);

    vout.normal = vin.normal;
    vout.uv = vin.uv;
    return vout;
}