#pragma once
#include "SceneObject.h"

// 基本図形情報クラス
enum KIND_BASICFIGURE
{
	BASICFIGURE_CUBE = 0,		// 立方体
	BASICFIGURE_SPHERE,			// 球
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
	SCENEOBJECTINFO m_ObjectInfo;		// オブジェクト情報
	KIND_BASICFIGURE m_kind;			// 図形の種類
};
