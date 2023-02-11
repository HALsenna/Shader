// �Q�[���V�[��
#pragma once
#include "SceneBase.hpp"
#include "UIManager.h"
#include "Model.h"
#include "ConstantBuffer.h"

class SceneGame : public SceneBase
{
public:
	void Init();		// ������
	void Uninit();		// �I��
	void Update();		// �X�V
	void Draw();		// �`��

private:
	void DrawShadow(Model* pModel, XMFLOAT4X4 matrix[], ConstantBuffer* pCB);	// ���f���̉e��`�悷��

private:
	CUIManager* m_UIManager;		// UI�}�l�[�W���[
	bool m_isLoad;
};

