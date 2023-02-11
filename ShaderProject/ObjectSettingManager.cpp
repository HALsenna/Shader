#include "ObjectSettingManager.h"

CSettingManager::CSettingManager()
{
}

CSettingManager::~CSettingManager()
{
}

void CSettingManager::Init()
{
	m_mode = SETTING_MEAT;		// ‚¨“÷‘I‘ğó‘Ô‚É‚·‚é
}

void CSettingManager::Update()
{
}

// ƒ‚[ƒh‚Ìİ’è
void CSettingManager::SetMode(MODE_SETTING mode)
{
	m_mode = mode;
}
