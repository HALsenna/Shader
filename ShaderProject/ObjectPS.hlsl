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

// 関数宣言
float3 calcPointLightDir(float4 world, float3 pointPos);		// 入射するポイントライトの光の向きを計算する
float calcAffect(float4 world, float3 pointPos, float ptRange);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 outColor;
	float4 texColor = tex.Sample(samp, pin.uv);		// テクスチャの反映
	outColor = texColor;
	outColor.xyz *= lightColor.xyz;

	float3 N = normalize(pin.normal);		// 面の法線
	float3 L = normalize(lightDir);			// 光源から面へのベクトル
	
	// 拡散反射光を計算する
	float diffuse = dot(N, L);	
	//outColor.xyz = outColor.xyz * diffuse;
	
	// アンビエントを設定する
	float Ambient = 0.3f;	
	outColor.xyz *= diffuse  * Ambient + Ambient;

	// 太陽光の色を設定する
	// 光の色を反映する
	//color *= float4(lightColor.x, lightColor.y, lightColor.z, 1.0f);
	return outColor;

	// 鏡面反射光を計算する
	// 反射ベクトルを求める
	float4 color;
	float3 refVec = reflect(L, N);
	// 視点ベクトルを求める
	float3 eyeVec = normalize(cameraPos.xyz - pin.wPos.xyz);
	// 鏡面反射の強さを求める
	//float specVal = max(0, dot(refVec, eyeVec));
	// 鏡面反射を絞る
	//specVal = pow(specVal, 10.0f);

	//	return float4(specVal, 0.0f, 0.0f, 0.0f);
	// 鏡面反射光を求める
	//float3 specular = specVal;

	// ポイントライトの方向を計算する
	float3 pointLightDir = normalize(calcPointLightDir(pin.wPos, ptPos));

	// ポイントライトをちらつかせる
	float lightPower = ptRange * sin(time / 60.0f);
	if (lightPower < 0.0f)
	{
		lightPower = -lightPower;
	}

	// ポイントライトの影響率を計算する
	float affect = calcAffect(pin.wPos, ptPos, lightPower);

	// 光を求める
	//float3 light = (diffuse + specular) + (affect * ptColor);

	float3 light = diffuse + (affect * ptColor);
	
	color.xyz = color.xyz * light.xyz;
	//color.xyz = color.xyz * diffuse * 0.8f;

	return color;
}

// ポイントライトの光の向きを計算する
float3 calcPointLightDir(float4 world, float3 pos)
{
	float3 _lightDir = world - pos;
	_lightDir = normalize(_lightDir);

	return _lightDir;
}

// 距離による影響率を計算する
float calcAffect(float4 world, float3 pos, float range)
{
	// 距離を計算する-
	float distance = length(world - pos);

	// 影響率を計算する
	float affect = 1.0f - 1.0f / range * distance;

	if (affect < 0.0f)
	{
		affect = 0.0f;
	}

	affect = pow(affect, 3.0f);

	return affect;
}