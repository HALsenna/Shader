#include "Light.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

CLight::CLight()
{
	XMFLOAT3 initPos = { -999.0f, -999.0f, -999.0f };		// 座標
	XMFLOAT3 initDir = { 0.0f, 1.0f, 0.0f };		// 方向
	XMFLOAT3 initColor = { 0.0f, 0.0f, 0.0f};				// 色
	m_posLight = { initPos, initDir, initColor };			// 太陽光初期化
}

CLight::~CLight()
{

}

void CLight::Init()
{
}

void CLight::Update()
{
	// 色の設定
	float setColor[4] = { m_posLight.color.x, m_posLight.color.y, m_posLight.color.z, 1.0f };
	ImGui::ColorEdit4(u8"LightColor", setColor);
	m_posLight.color = { setColor[0], setColor[1], setColor[2]};

	// 方向の設定
	float setDir[3] = { m_posLight.direction.x, m_posLight.direction.y, m_posLight.direction.z };
	ImGui::SliderFloat3("direction", setDir, -1.0f, 1.0f);
	m_posLight.direction = { setDir[0], setDir[1], setDir[2] };
}

void CLight::Draw()
{
}

// 太陽光の色の設定
void CLight::SetSkyColor(XMFLOAT3 color)
{
	m_posLight.color = color;
}

// 太陽光の色情報の取得
XMFLOAT3 CLight::GetSkyColor()
{
	return m_posLight.color;
}

// 太陽光の方向の取得
void CLight::SetSkyLightDir(XMFLOAT3 direction)
{
	m_posLight.direction = direction;
}

// 太陽光の方向の取得
XMFLOAT3 CLight::GetSkyLightDir()
{
	return m_posLight.direction;
}
