#include "Light.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

CLight::CLight()
{
	XMFLOAT3 initPos = { -999.0f, -999.0f, -999.0f };		// À•W
	XMFLOAT3 initDir = { 0.0f, 1.0f, 0.0f };		// •ûŒü
	XMFLOAT3 initColor = { 0.0f, 0.0f, 0.0f};				// F
	m_posLight = { initPos, initDir, initColor };			// ‘¾—zŒõ‰Šú‰»
}

CLight::~CLight()
{

}

void CLight::Init()
{
}

void CLight::Update()
{
	// F‚Ìİ’è
	float setColor[4] = { m_posLight.color.x, m_posLight.color.y, m_posLight.color.z, 1.0f };
	ImGui::ColorEdit4(u8"LightColor", setColor);
	m_posLight.color = { setColor[0], setColor[1], setColor[2]};

	// •ûŒü‚Ìİ’è
	float setDir[3] = { m_posLight.direction.x, m_posLight.direction.y, m_posLight.direction.z };
	ImGui::SliderFloat3("direction", setDir, -1.0f, 1.0f);
	m_posLight.direction = { setDir[0], setDir[1], setDir[2] };
}

void CLight::Draw()
{
}

// ‘¾—zŒõ‚ÌF‚Ìİ’è
void CLight::SetSkyColor(XMFLOAT3 color)
{
	m_posLight.color = color;
}

// ‘¾—zŒõ‚ÌFî•ñ‚Ìæ“¾
XMFLOAT3 CLight::GetSkyColor()
{
	return m_posLight.color;
}

// ‘¾—zŒõ‚Ì•ûŒü‚Ìæ“¾
void CLight::SetSkyLightDir(XMFLOAT3 direction)
{
	m_posLight.direction = direction;
}

// ‘¾—zŒõ‚Ì•ûŒü‚Ìæ“¾
XMFLOAT3 CLight::GetSkyLightDir()
{
	return m_posLight.direction;
}
