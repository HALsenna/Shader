struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 screenPos : TEXCOORD0;
};

float main(PS_IN pin) : SV_TARGET
{
	float depth = pin.screenPos.z / pin.screenPos.w;		// 深度値の書き込み
	return depth;
}