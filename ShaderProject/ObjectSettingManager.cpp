#include "ObjectSettingManager.h"

CSettingManager::CSettingManager()
{
}

CSettingManager::~CSettingManager()
{
}

void CSettingManager::Init()
{
	m_mode = SETTING_MEAT;		// お肉選択状態にする
}

void CSettingManager::Update()
{
}

// モードの設定
void CSettingManager::SetMode(MODE_SETTING mode)
{
	m_mode = mode;
}
