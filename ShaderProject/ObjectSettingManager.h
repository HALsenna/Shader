#pragma once

enum MODE_SETTING
{
	SETTING_MEAT = 0,		// �����̃I�u�W�F�N�g�ݒ�
};

class CSettingManager
{
public:
	CSettingManager();
	~CSettingManager();
	void Init();
	void Update();
	void SetMode(MODE_SETTING mode);		// ���[�h�̐ݒ�

private:
	MODE_SETTING m_mode;
};
