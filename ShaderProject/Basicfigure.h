#pragma once
#include "SceneObject.h"

// ��{�}�`���N���X
enum KIND_BASICFIGURE
{
	BASICFIGURE_CUBE = 0,		// ������
	BASICFIGURE_SPHERE,			// ��
	BASICFIGURE_MAX,
};
class CBasicFigure
{
public:
	CBasicFigure(SCENEOBJECTINFO info, KIND_BASICFIGURE kind);
	~CBasicFigure();
	void Init();
	void Update();
private:
	SCENEOBJECTINFO m_ObjectInfo;		// �I�u�W�F�N�g���
	KIND_BASICFIGURE m_kind;			// �}�`�̎��
};
