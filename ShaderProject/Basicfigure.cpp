#include "Basicfigure.h"

const char* szModelFileName[BASICFIGURE_MAX] = {
	"Assets/Model/BasicFigure/BasicFigure_Cube.fbx",		// ������
	"Assets/Model/BasicFigure/BasicFigure_Sphere.fbx",		// ����
};

// �}�`�̐���
CBasicFigure::CBasicFigure(SCENEOBJECTINFO info, KIND_BASICFIGURE kind)
{
	// �I�u�W�F�N�g�����i�[����
	m_ObjectInfo = info;
	m_kind = kind;

	Init();
}

CBasicFigure::~CBasicFigure()
{
}

void CBasicFigure::Init()
{
	Model* pModel = m_ObjectInfo.pModel;
	// ���f���ɏ���ݒ肷��
	pModel->Load(szModelFileName[m_kind], m_ObjectInfo.size.x);
	pModel->SetPos(m_ObjectInfo.pos);		// ���W
	pModel->SetSize(m_ObjectInfo.size);		// �g�嗦
	pModel->SetRot(m_ObjectInfo.rot);		// ��]
	pModel->SetVertexShader(GetVS(m_ObjectInfo.vs));	// ���_�V�F�[�_�[
	pModel->SetPixelShader(GetPS(m_ObjectInfo.ps));		// �s�N�Z���V�F�[�_�[
}

void CBasicFigure::Update()
{
}
