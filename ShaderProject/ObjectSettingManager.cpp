#include "ObjectSettingManager.h"

CSettingManager::CSettingManager()
{
}

CSettingManager::~CSettingManager()
{
}

void CSettingManager::Init()
{
	m_mode = SETTING_MEAT;		// �����I����Ԃɂ���
}

void CSettingManager::Update()
{
}

// ���[�h�̐ݒ�
void CSettingManager::SetMode(MODE_SETTING mode)
{
	m_mode = mode;
}
