#include "Input.h"

//--- �O���[�o���ϐ�
BYTE g_keyTable[256];
BYTE g_oldTable[256];

HRESULT InitInput()
{
	// ��ԍŏ��̓���
	GetKeyboardState(g_keyTable);
	return S_OK;
}
void UninitInput()
{
}
void UpdateInput()
{
	// �Â����͂��X�V
	memcpy_s(g_oldTable, sizeof(g_oldTable), g_keyTable, sizeof(g_keyTable));
	// ���݂̓��͂��擾
	GetKeyboardState(g_keyTable);
}

DirectX::XMFLOAT2 GetWindowCursorPos()
{
	POINT poCursorPos;
	GetCursorPos(&poCursorPos);
	ScreenToClient(FindWindowA(nullptr, APP_TITLE), &poCursorPos);

	DirectX::XMFLOAT2 fPos = { (float)poCursorPos.x, (float)poCursorPos.y };

	// �}�E�X�̍��W�͍��オ���_�ł��邽�߁A��ʂ̒��S�����_�̏ꍇ�̍��W�ɕϊ�����
	fPos.x -= SCREEN_WIDTH / 2;
	fPos.y -= SCREEN_HEIGHT / 2;
	fPos.y *= -1.0f;

	return fPos;
}

bool IsKeyPress(BYTE key)
{
	return g_keyTable[key] & 0x80;
}
bool IsKeyTrigger(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_keyTable[key] & 0x80;
}
bool IsKeyRelease(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_oldTable[key] & 0x80;
}
bool IsKeyRepeat(BYTE key)
{
	return false;
}