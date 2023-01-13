#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class CFlower
{
public:
	CFlower();
	~CFlower();
	void Init();
	void Update();
	void Draw();
	void ChangeColor(XMFLOAT4 color);		// 花びらの色を変更する

public:
	enum FLOWERKIND
	{

	};

private:
	XMFLOAT4 m_color = {1.0f, 1.0f, 1.0f, 1.0f};
	
};
