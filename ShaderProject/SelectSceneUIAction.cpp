// �Z���N�g�V�[���ł�UI�̓���
#include "SelectSceneUIAction.h"
#include "UIObj.h"		// UI�̃I�u�W�F�N�g���𗘗p���邽��

void SelectMeat(CUIManager* manager)
{
	// UI��\��������
	CUI* pUIObj;

	// �ݒ��ʂ�\������
	pUIObj = manager->GetUIObj(SELECT_SETTING_BG);
	pUIObj->SetActive(true);

}
