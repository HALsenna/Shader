struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};

cbuffer Light : register(b0)
{
	float3 lightDir;
	float dummy;
	//float power;
	float4 lightColor;
	float smooth;
	float3 dummy2;
};

cbuffer Camera : register(b1)
{
	float4 cameraPos;
}

cbuffer LightPower : register(b2)
{
	float Power;
	float3 dummy4;
}

cbuffer pointLight : register(b4)
{
	float3 ptPos;
	float ptDummy;
	float4 ptColor;
	float ptRange;
	float3 ptDummy2;
}

cbuffer time : register(b5)
{
	float time;
	float3 timeDummy;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// �֐��錾
float3 calcPointLightDir(float4 world, float3 pointPos);		// ���˂���|�C���g���C�g�̌��̌������v�Z����
float calcAffect(float4 world, float3 pointPos, float ptRange);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 outColor;
	float4 texColor = tex.Sample(samp, pin.uv);		// �e�N�X�`���̔��f
	outColor = texColor;
	outColor.xyz *= lightColor.xyz;

	float3 N = normalize(pin.normal);		// �ʂ̖@��
	float3 L = normalize(lightDir);			// ��������ʂւ̃x�N�g��
	
	// �g�U���ˌ����v�Z����
	float diffuse = dot(N, L);	
	//outColor.xyz = outColor.xyz * diffuse;
	
	// �A���r�G���g��ݒ肷��
	float Ambient = 0.3f;	
	outColor.xyz *= diffuse  * Ambient + Ambient;

	// ���z���̐F��ݒ肷��
	// ���̐F�𔽉f����
	//color *= float4(lightColor.x, lightColor.y, lightColor.z, 1.0f);
	return outColor;

	// ���ʔ��ˌ����v�Z����
	// ���˃x�N�g�������߂�
	float4 color;
	float3 refVec = reflect(L, N);
	// ���_�x�N�g�������߂�
	float3 eyeVec = normalize(cameraPos.xyz - pin.wPos.xyz);
	// ���ʔ��˂̋��������߂�
	//float specVal = max(0, dot(refVec, eyeVec));
	// ���ʔ��˂��i��
	//specVal = pow(specVal, 10.0f);

	//	return float4(specVal, 0.0f, 0.0f, 0.0f);
	// ���ʔ��ˌ������߂�
	//float3 specular = specVal;

	// �|�C���g���C�g�̕������v�Z����
	float3 pointLightDir = normalize(calcPointLightDir(pin.wPos, ptPos));

	// �|�C���g���C�g�����������
	float lightPower = ptRange * sin(time / 60.0f);
	if (lightPower < 0.0f)
	{
		lightPower = -lightPower;
	}

	// �|�C���g���C�g�̉e�������v�Z����
	float affect = calcAffect(pin.wPos, ptPos, lightPower);

	// �������߂�
	//float3 light = (diffuse + specular) + (affect * ptColor);

	float3 light = diffuse + (affect * ptColor);
	
	color.xyz = color.xyz * light.xyz;
	//color.xyz = color.xyz * diffuse * 0.8f;

	return color;
}

// �|�C���g���C�g�̌��̌������v�Z����
float3 calcPointLightDir(float4 world, float3 pos)
{
	float3 _lightDir = world - pos;
	_lightDir = normalize(_lightDir);

	return _lightDir;
}

// �����ɂ��e�������v�Z����
float calcAffect(float4 world, float3 pos, float range)
{
	// �������v�Z����-
	float distance = length(world - pos);

	// �e�������v�Z����
	float affect = 1.0f - 1.0f / range * distance;

	if (affect < 0.0f)
	{
		affect = 0.0f;
	}

	affect = pow(affect, 3.0f);

	return affect;
}