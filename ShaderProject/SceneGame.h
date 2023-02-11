// ゲームシーン
#pragma once
#include "SceneBase.hpp"
#include "UIManager.h"
#include "Model.h"
#include "ConstantBuffer.h"

class SceneGame : public SceneBase
{
public:
	void Init();		// 初期化
	void Uninit();		// 終了
	void Update();		// 更新
	void Draw();		// 描画

private:
	void DrawShadow(Model* pModel, XMFLOAT4X4 matrix[], ConstantBuffer* pCB);	// モデルの影を描画する

private:
	CUIManager* m_UIManager;		// UIマネージャー
	bool m_isLoad;
};

