#pragma once
#include "Geometory.h"
#include "DirectX.h"
#include "UIList.h"
#include "Texture.h"
#include "ShaderList.h"

using namespace DirectX;


// UI�̎��
enum UIKIND
{
	UI_SLIDER = 0,		// �X���C�_�[
	UI_BUTTON,			// �{�^��
	UI_BACKGROUND,		// �w�i
	UI_NONE,

	UI_MAX
};

// UI�`��ɕK�v�ȏ��
struct UIINFO
{
	XMFLOAT3 pos;
	XMFLOAT2 size;
	XMFLOAT4 color;
	Texture* pTex;
	UIKIND kind;
	SELECTUIKIND UIName;
	PixelShaderKind ps;
	VertexShaderKind vs;
};
//std::tuple<XMFLOAT3, XMFLOAT2, XMFLOAT4, Texture*, UIKIND, const char*> 


// UI���o
enum STAGINGSTATE
{
	STAGING_FADEIN = 0,		// �t�F�[�h�C��
	STAGING_FADEOUT,		// �t�F�[�h�A�E�g
	STAGING_FLASH,			// �t���b�V��
	STAGING_DISSOLOVE,		// �f�B�]���u���o
	STAGING_NONE,			// ���o����
	
	STAGING_MAX
};

class CUI
{
public:
	UIINFO m_uiInfo;
protected:
	bool m_isActive;			// �\������
	bool m_isSelected;			// �I������Ă��邩
private:
	CGeometory* m_pGeometory;
	STAGINGSTATE m_state;		// �t�F�[�h�̏��
	float m_fFadeSec;			// �`��J�n/�I�����Ƀt�F�[�h���邩�ǂ���
	float m_fFlashSpeed;		// �_�ŃX�s�[�h
	float m_fTime;
	float m_dissolveRate;
	float m_dissolveTime;
	bool m_bFlashFadeIn;	
	//Texture* m_pTex;			// �e�N�X�`��
	Texture* m_pDissolveTex;	// �f�B�]���u�e�N�X�`��
	//UIKIND m_kind;
public:
	CUI();
	~CUI();
	void Init(UIINFO data);
	virtual void Update();
	void Draw();
	void Fade(STAGINGSTATE state, float fadesec);
	void Flash(float flashSpeed);
	void Dissolve(float dissolvesec, Texture* pTex);
	void SetPos(XMFLOAT3 pos);
	XMFLOAT3 GetPos();
	bool IsCursorHit(XMFLOAT2 UIPos, XMFLOAT2 size);
	void SetActive(bool flg);		// �\��or��\����ݒ肷��
	bool GetSelected() { return m_isSelected; };		// �I������Ă��邩��n��
};
