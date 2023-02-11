#ifndef __INPUT_H__
#define __INPUT_H__

#include <Windows.h>
#include "Defines.h"
#include <DirectXMath.h>

HRESULT InitInput();
void UninitInput();
void UpdateInput();
DirectX::XMFLOAT2 GetWindowCursorPos();

bool IsKeyPress(BYTE key);
bool IsKeyTrigger(BYTE key);
bool IsKeyRelease(BYTE key);
bool IsKeyRepeat(BYTE key);


#endif // __INPUT_H__