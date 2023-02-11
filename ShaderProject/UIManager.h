#pragma once
#include "UIObj.h"
#include <vector>

class CUIManager
{
public:
	CUIManager();
	~CUIManager();
	void Init();
	void Update();
	CUI* CreateUI(int kind, UIINFO data);
	CUI* GetUIObj(SELECTUIKIND kind);		// UIのオブジェクト情報を取得する
	CUI* AddUI(UIINFO data);
private:
	CUI* m_pUIObj[SELECT_MAX];
	std::vector<CUI*> m_aUIObj;
};
