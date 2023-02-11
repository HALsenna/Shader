#include "UIButton.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include <DirectXMath.h>
#include "Input.h"

UIButton::UIButton()
{

}

UIButton::~UIButton()
{
}

void UIButton::Init()
{
	
}

void UIButton::Update()
{
	if (!m_isActive) return;
	// UI�̍��W��񎟌��ɕύX����
	DirectX::XMFLOAT2 fUIPos = { m_uiInfo.pos.x , m_uiInfo.pos.y };
	ImGui::Text("UIPos : %.2f, %.2f", fUIPos.x, fUIPos.y);

	// UI�̑傫�����擾����
	DirectX::XMFLOAT2 fUISize = { (float)m_uiInfo.pTex->GetWidth(), (float)m_uiInfo.pTex->GetHeight() };
	ImGui::Text("UISize : %.2f, %.2f", fUISize.x, fUISize.y);

	// �}�E�X���d�Ȃ��Ă�����
	if (IsCursorHit(fUIPos, fUISize))
	{
		ImGui::Text("HIT");
		m_uiInfo.color = { 0.48f, 1.0f, 0.52f, 1.0f };

		// ���N���b�N�őI�����ꂽ�ꍇ���ꂼ��̏��������s����
		if (IsKeyPress(VK_LBUTTON))
		{
			m_uiInfo.color = { 1.0f, 0.0f, 0.0f, 1.0f };
			m_isSelected = true;
		}
		else
		{
			m_isSelected = false;
		}
	}
	else
	{
		m_uiInfo.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_isSelected = false;
	}

}

