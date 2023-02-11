// セレクトシーンでのUIの動き
#include "SelectSceneUIAction.h"
#include "UIObj.h"		// UIのオブジェクト情報を利用するため

void SelectMeat(CUIManager* manager)
{
	// UIを表示させる
	CUI* pUIObj;

	// 設定画面を表示する
	pUIObj = manager->GetUIObj(SELECT_SETTING_BG);
	pUIObj->SetActive(true);

}
