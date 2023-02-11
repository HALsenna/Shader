#include "UIObj.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "Input.h"

//UIButton* g_pButton;



CUI::CUI()
{
	
}

CUI::~CUI()
{
}

void CUI::Init(UIINFO data)
{
	//g_pButton = new UIButton();
	m_uiInfo = data;
	m_uiInfo.size = XMFLOAT2(data.pTex->GetWidth() * data.size.x, data.pTex->GetHeight() * data.size.y);

	m_fTime = 0.0f;
	m_dissolveRate = 0.0f;
	// �|���S���̐���
	m_pGeometory = new CGeometory();
	m_pGeometory->InitGeometory();		// �I�u�W�F�N�g�̐���
	// �V�F�[�_�[�̐ݒ�
	m_pGeometory->Set2DPixelShader(GetPS(m_uiInfo.ps));
	m_pGeometory->Set2DVertexShader(GetVS(m_uiInfo.vs));

	m_state = STAGING_NONE;
	m_isActive = true;
	m_isSelected = false;
}

void CUI::Update()
{
	if (!m_isActive) return;
	// �^�C�}�[�X�V
	m_fTime += 1.0f / 60.0f;

	//g_pButton->Update();

	// ���o
	#pragma region staging
	switch (m_state)
	{
	// �t�F�[�h�C��
	case STAGING_FADEIN:
		// �F���̃Z�b�g
		m_uiInfo.color.w = m_fTime / m_fFadeSec;

		// ���ԂɂȂ����牉�o�I��
		if (m_fTime > m_fFadeSec)
		{
			m_fTime = 0.0f;
			m_state = STAGING_NONE;
		}
		break;
	
	// �t�F�[�h�A�E�g
	case STAGING_FADEOUT:
		// �F���̃Z�b�g
		m_uiInfo.color.w = 1.0f - m_fTime / m_fFadeSec;

		// ���ԂɂȂ����牉�o�I��
		if (m_fTime > m_fFadeSec)
		{
			m_fTime = 0.0f;
			m_state = STAGING_NONE;
		}
		break;

	case STAGING_FLASH:
		// �F���̃Z�b�g
		if (m_bFlashFadeIn)
			m_uiInfo.color.w = 1.0f - m_fTime / m_fFlashSpeed;
		else
			m_uiInfo.color.w = m_fTime / m_fFlashSpeed;

		// ���ԂɂȂ����牉�o�I��
		if (m_fTime > m_fFlashSpeed)
		{
			m_fTime = 0.0f;
			m_bFlashFadeIn = !m_bFlashFadeIn;
		}

		break;

	// �f�B�]���u���o
	case STAGING_DISSOLOVE:
		m_dissolveRate = 1.0f - m_fTime / m_dissolveTime;
		m_pGeometory->SetDissolve(m_dissolveRate, m_pDissolveTex->GetResource());
		break;
	case STAGING_NONE:
		break;
	case STAGING_MAX:
		break;
	default:
		break;
	}
	#pragma endregion
}

void CUI::Draw()
{
	if (!m_isActive) return;

	m_pGeometory->SetTexture(m_uiInfo.pTex->GetResource());
	m_pGeometory->SetColor(m_uiInfo.color);
	m_pGeometory->DrawPolygon(m_uiInfo.pos, m_uiInfo.size, XMFLOAT3(0.0f, 0.0f, 0.0f));
}

// �t�F�[�h����
void CUI::Fade(STAGINGSTATE state, float fadesec)
{
	m_state = state;
	m_fTime = 0.0f;
	m_fFadeSec = fadesec;
}

// �t���b�V������
void CUI::Flash(float flashSpeed)
{
	m_state = STAGING_FLASH;
	m_bFlashFadeIn = true;
	m_fFlashSpeed = flashSpeed;
}

// �f�B�]���u����
void CUI::Dissolve(float dissolvesec, Texture* pTex)
{
	m_state = STAGING_DISSOLOVE;
	m_dissolveTime = dissolvesec;
	m_pDissolveTex = pTex;
}

void CUI::SetPos(XMFLOAT3 pos)
{
	m_uiInfo.pos = pos;
}

XMFLOAT3 CUI::GetPos()
{
	return m_uiInfo.pos;
}

bool CUI::IsCursorHit(XMFLOAT2 UIPos, XMFLOAT2 size)
{
	bool flg;
	XMFLOAT2 fCursurPos = GetWindowCursorPos();		// �J�[�\�����W
	XMFLOAT2 fHitPosMin = { UIPos.x - size.x / 2, UIPos.y + size.y / 2};		// �q�b�g����J�[�\�����W�̍ŏ��l
	XMFLOAT2 fHitPosMax = { UIPos.x + size.x / 2, UIPos.y - size.y / 2};		// �q�b�g����J�[�\�����W�̍ő�l
	ImGui::Text("fHitPosMin : %.2f, %.2f", fHitPosMin.x, fHitPosMin.y);
	ImGui::Text("fHitPosMax : %.2f, %.2f", fHitPosMax.x, fHitPosMax.y);

	// X���W�Ɋւ��锻��
	if (fHitPosMin.x < fCursurPos.x && fCursurPos.x < fHitPosMax.x)
	{
		// Y���W�Ɋւ��锻��
		if (fHitPosMax.y < fCursurPos.y && fCursurPos.y < fHitPosMin.y)
			flg = true;
		else
			flg = false;
	}
	else
	{
		flg = false;
	}
	return flg;
}

void CUI::SetActive(bool flg)
{
	m_isActive = flg;
}
