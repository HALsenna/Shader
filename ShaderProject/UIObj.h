#pragma once
#include "Geometory.h"
#include "DirectX.h"
#include "UIList.h"
#include "Texture.h"
#include "ShaderList.h"

using namespace DirectX;


// UIの種類
enum UIKIND
{
	UI_SLIDER = 0,		// スライダー
	UI_BUTTON,			// ボタン
	UI_BACKGROUND,		// 背景
	UI_NONE,

	UI_MAX
};

// UI描画に必要な情報
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


// UI演出
enum STAGINGSTATE
{
	STAGING_FADEIN = 0,		// フェードイン
	STAGING_FADEOUT,		// フェードアウト
	STAGING_FLASH,			// フラッシュ
	STAGING_DISSOLOVE,		// ディゾルブ演出
	STAGING_NONE,			// 演出無し
	
	STAGING_MAX
};

class CUI
{
public:
	UIINFO m_uiInfo;
protected:
	bool m_isActive;			// 表示中か
	bool m_isSelected;			// 選択されているか
private:
	CGeometory* m_pGeometory;
	STAGINGSTATE m_state;		// フェードの状態
	float m_fFadeSec;			// 描画開始/終了時にフェードするかどうか
	float m_fFlashSpeed;		// 点滅スピード
	float m_fTime;
	float m_dissolveRate;
	float m_dissolveTime;
	bool m_bFlashFadeIn;	
	//Texture* m_pTex;			// テクスチャ
	Texture* m_pDissolveTex;	// ディゾルブテクスチャ
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
	void SetActive(bool flg);		// 表示or非表示を設定する
	bool GetSelected() { return m_isSelected; };		// 選択されているかを渡す
};
