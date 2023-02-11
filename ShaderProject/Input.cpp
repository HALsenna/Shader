#include "Input.h"

//--- グローバル変数
BYTE g_keyTable[256];
BYTE g_oldTable[256];

HRESULT InitInput()
{
	// 一番最初の入力
	GetKeyboardState(g_keyTable);
	return S_OK;
}
void UninitInput()
{
}
void UpdateInput()
{
	// 古い入力を更新
	memcpy_s(g_oldTable, sizeof(g_oldTable), g_keyTable, sizeof(g_keyTable));
	// 現在の入力を取得
	GetKeyboardState(g_keyTable);
}

DirectX::XMFLOAT2 GetWindowCursorPos()
{
	POINT poCursorPos;
	GetCursorPos(&poCursorPos);
	ScreenToClient(FindWindowA(nullptr, APP_TITLE), &poCursorPos);

	DirectX::XMFLOAT2 fPos = { (float)poCursorPos.x, (float)poCursorPos.y };

	// マウスの座標は左上が原点であるため、画面の中心が原点の場合の座標に変換する
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