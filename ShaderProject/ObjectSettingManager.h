#pragma once

enum MODE_SETTING
{
	SETTING_MEAT = 0,		// お肉のオブジェクト設定
};

class CSettingManager
{
public:
	CSettingManager();
	~CSettingManager();
	void Init();
	void Update();
	void SetMode(MODE_SETTING mode);		// モードの設定

private:
	MODE_SETTING m_mode;
};
