#include "Basicfigure.h"

const char* szModelFileName[BASICFIGURE_MAX] = {
	"Assets/Model/BasicFigure/BasicFigure_Cube.fbx",		// 立方体
	"Assets/Model/BasicFigure/BasicFigure_Sphere.fbx",		// 球体
};

// 図形の生成
CBasicFigure::CBasicFigure(SCENEOBJECTINFO info, KIND_BASICFIGURE kind)
{
	// オブジェクト情報を格納する
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
	// モデルに情報を設定する
	pModel->Load(szModelFileName[m_kind], m_ObjectInfo.size.x);
	pModel->SetPos(m_ObjectInfo.pos);		// 座標
	pModel->SetSize(m_ObjectInfo.size);		// 拡大率
	pModel->SetRot(m_ObjectInfo.rot);		// 回転
	pModel->SetVertexShader(GetVS(m_ObjectInfo.vs));	// 頂点シェーダー
	pModel->SetPixelShader(GetPS(m_ObjectInfo.ps));		// ピクセルシェーダー
}

void CBasicFigure::Update()
{
}
