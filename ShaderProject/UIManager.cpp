#include "UIManager.h"
#include "UIButton.h"
#include "Input.h"			// �L�[����
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "SelectSceneUIAction.h" 


CUIManager::CUIManager()
{
	
}

CUIManager::~CUIManager()
{

}

void CUIManager::Init()
{
}

void CUIManager::Update()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		//m_pUIObj[i]->Update();
		m_aUIObj[i]->Update();

		// UI���I������Ă������ނɉ����ăA�N�V��������
		//if (m_pUIObj[i]->GetSelected())
		if (m_aUIObj[i]->GetSelected())
		{
			switch (i)
			{
				// ����
				case SELECT_MEAT:
					SelectMeat(this);
					break;
				default:
					break;
			}
		}
			
	}

	// �J�[�\�����W�̃f�o�b�O�\��
	XMFLOAT2 cursorPos = GetWindowCursorPos();
	ImGui::Text("CursurPos : %.2f, %.2f", cursorPos.x, cursorPos.y);
}

CUI* CUIManager::CreateUI(int kind, UIINFO data)
{
	m_pUIObj[kind] = new CUI();

	switch (data.kind)
	{	
		// �X���C�_�[(������)
		case UI_SLIDER:
			break;
		// �{�^��
		case UI_BUTTON:
			UIButton* test;
			test = new UIButton();
			m_pUIObj[kind] = test;
			break;
		case UI_BACKGROUND:
			break;
		case UI_NONE:
			break;
	}
	
	m_pUIObj[kind]->Init(data);
	return m_pUIObj[kind];
}

CUI* CUIManager::GetUIObj(SELECTUIKIND kind)
{
	return m_aUIObj[(int)kind];
}

CUI* CUIManager::AddUI(UIINFO data)
{
	CUI* pObj = new CUI;
	m_aUIObj.push_back(pObj);

	switch (data.kind)
	{
		// �X���C�_�[(������)
	case UI_SLIDER:
		break;
		// �{�^��
	case UI_BUTTON:
		UIButton* test;
		test = new UIButton();
		pObj = test;
		break;
	case UI_BACKGROUND:
		break;
	case UI_NONE:
		break;
	}
	pObj->Init(data);
	return pObj;
}
