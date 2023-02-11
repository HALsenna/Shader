#pragma once
// �v���C���[�N���X
#include "Model.h"		// ���f��
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