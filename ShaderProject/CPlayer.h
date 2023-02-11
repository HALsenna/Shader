#pragma once
// プレイヤークラス
#include "Model.h"		// モデル
#include <DirectXMath.h>
class CNewPlayer
{
public:
	void Init(Model* pModel, DirectX::XMFLOAT3 initPos);
	void Update();
	void Draw();
private:
	Model* m_pModel;
	DirectX::XMFLOAT3 m_pos;
};