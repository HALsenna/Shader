#include "SceneSelect.h"
#include "Texture.h"
#include "TextureFactory.h"
#include "CBlendState.h"
#include "CameraBase.h"
#include "Input.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

// èè`
namespace
{
	const float BUTTON_SIZE = 0.4f;
	const float KEYUI_SIZE = 0.3f;
	const float KEYUI_POSY = -300.0f;
	const float KEYUI_CENTER_POSX = -200.0f;
	const float KEYUI_INTERVAL = 50.0f;
}

// ---- UIÌîñ ----
#pragma region UIInitInfo
// t@C¼
static const char* szUIFileName[SELECT_MAX] = {
	"Assets/Texture/MenuUI/TEX_BG_Plane.png",		// wi
	"Assets/Texture/MenuUI/TEX_Menu_Meat.png",		// IðÚ(¨÷)
	"Assets/Texture/MenuUI/TEX_Menu_UI.png",		// IðÚ(UI)
	"Assets/Texture/MenuUI/TEX_Setting_BG.png",		// ÝèÚ(wi)
	"Assets/Texture/MenuUI/TEX_Button_Decide.png",	// è{^
	"Assets/Texture/MenuUI/TEX_Key_1.png",			// L[Num1
	"Assets/Texture/MenuUI/TEX_Key_2.png",			// L[Num2
	"Assets/Texture/MenuUI/TEX_Key_3.png",			// L[Num3
	"Assets/Texture/MenuUI/TEX_ChangeLight.png",	// CgØÖUI
};

// UIîñ
//{0.48f, 1.0f, 0.52f, 1.0f},
const UIINFO g_UIInfo[SELECT_MAX] = {
	{{0.0f, 0.0f, 0.0f},{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_BACKGROUND},		// wi
	{{-500.0f, 250.0f, 0.0f},{BUTTON_SIZE, BUTTON_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_BUTTON},		// IðÚ(¨÷)
	{{-500.0f, 180.0f, 0.0f},{BUTTON_SIZE, BUTTON_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_BUTTON},		// IðÚ(UI)
	{{400.0f, 50.0f, 0.0f},{0.4f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_BACKGROUND},	// IðÚ(wi)
	{{400.0f, -100.0f, 0.0f},{0.4f, 0.4f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_BUTTON},	// è{^
	{{KEYUI_CENTER_POSX - KEYUI_INTERVAL, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_NONE},		// L[Num1
	{{KEYUI_CENTER_POSX, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_NONE},						// L[Num2
	{{KEYUI_CENTER_POSX + KEYUI_INTERVAL, KEYUI_POSY, 0.0f},{KEYUI_SIZE, KEYUI_SIZE}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_NONE},		// L[Num3
	{{50.0f, KEYUI_POSY, 0.0f},{0.7f, 0.7f}, {1.0f, 1.0f, 1.0f, 1.0f}, nullptr, UI_NONE},		// CgØÖUI
};
#pragma endregion
// ------------------

// ---- fÌúîñ ----
#pragma region ModelInitInfo
// IuWFNg¼
const char* g_szObjectName[SELECTOBJ_MAX] = {
	"Meat",		// ¨÷
	"DebugSphere",		// fobOp
};

// ÀW
const XMFLOAT3 g_ObjectPos[SELECTOBJ_MAX] = {
	{0.0f, 0.0f, 0.0f},		// ¨÷
	{0.5f, 3.0f, 0.0f},		// fobOp
};

// ñ]
const XMFLOAT3 g_ObjectRot[SELECTOBJ_MAX] = {
	{0.0f, 0.0f, 0.0f},		// ¨÷
	{0.0f, 0.0f, 0.0f},		// fobOp
};

// TCY
const XMFLOAT3 g_ObjectSize[SELECTOBJ_MAX] = {
	{1.0f, 1.0f, 1.0f},					// ¨÷
	{0.5f, 0.5f, 0.5f},					// fobOp
};

// ¸_VF[_[
const VertexShaderKind g_vShader[SELECTOBJ_MAX] = {
	VS_OBJECT,				// ¨÷
	VS_OBJECT,				// fobOp
};

// sNZVF[_[
const PixelShaderKind g_pShader[SELECTOBJ_MAX] = {
	PS_OBJECT,				// ¨÷
	PS_OBJECT,				// fobOp
};
#pragma endregion 
// -----------------------

// ---- üÍL[ ----
#pragma region inputkey
namespace
{
	const BYTE KEY_CHANGELIGHT_NORMAL = '1';	// ÊíóÔ
	const BYTE KEY_CHANGELIGHT_INSIDE =	'2';	// ¨à
	const BYTE KEY_CHANGELIGHT_NIGHT = '3';		// é	
}
#pragma endregion 
// ----------------

// O[oÏ
static CMeat_Setting* g_pMeatObj;		// ¨÷IuWFNg
static CBasicFigure* g_pBasicFig;		// ¨÷IuWFNg

// vg^Cvé¾
UIINFO SetUpUI(UIINFO);
//CUI* CreateUI(UIINFO);

void SceneSelect::Init()
{
	// UIÌú»
	InitUI();
	// 3DÌú»
	Init3D();
	// Â«õÌú»
	InitLight();
	
}
void SceneSelect::Uninit()
{
	m_pObjData.clear();
	m_pObjData.clear();
}
void SceneSelect::Update()
{
	// UIÌXV
	/*for (int i = 0; i < SELECT_MAX; ++i)
	{
		m_pUIObj[i]->Update();
	}*/
	m_UIManager->Update();

	// L[üÍótXV
	UpdateKeyInput();
	// ¨÷XV
	g_pMeatObj->Update();
}
void SceneSelect::Draw()
{
	// At@uhÝè
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);
	
	// wi`æ
	m_pUIObj[SELECT_BG_PLANE]->Draw();
	
	// At@uhÝèð³Éß·
	CBlendState::SetZBuffer(true);
	
	// 3DIuWFNgÌ`æ
	for (int i = 0; i < m_pObjData.size(); ++i)
	{
		SetShader((SELECTSCENE_OBJECT) i);			// VF[_[ÌÝè
		m_pObjData[i]->Draw();
	}

	// At@uhÝè
	CBlendState::SetZBuffer(false);
	CBlendState::SetBlendState(BS_ALPHABLEND);

	// UIÌ`æ
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		// wiÈOð`æ·é
		if (i != (int)SELECT_BG_PLANE)
			m_pUIObj[i]->Draw();
	}
	
	// At@uhÝèð³Éß·
	CBlendState::SetZBuffer(true);
}

/*
	@brief UIÌú»
	@details UIÌÀWAFAeNX`ðÝèµAIuWFNgð¶¬·é
*/
void SceneSelect::InitUI()
{
	m_UIManager = new CUIManager();
	Texture* pTex;
	for (int i = 0; i < SELECT_MAX; ++i)
	{
		// UIÌúÝè
		m_aUIInitData[i] = SetUpUI(g_UIInfo[i]);
		pTex = TextureFactory::CreateFromFile(szUIFileName[i]);
		m_aUIInitData[i].pTex = pTex;
		m_aUIInitData[i].UIName = (SELECTUIKIND)i;
		m_aUIInitData[i].ps = PS_2D;	// sNZVF[_[
		m_aUIInitData[i].vs = VS_2D;	// ¸_VF[_[
		// UIÌ¶¬
		//m_pUIObj[i] = m_UIManager->CreateUI(i, m_aUIInitData[i]);
		m_pUIObj[i] = m_UIManager->AddUI(m_aUIInitData[i]);
	}

	// nßÉ\¦·éUIÌÝè
	m_pUIObj[SELECT_BG_PLANE]->SetActive(true);		// wi
	m_pUIObj[SELECT_MEAT]->SetActive(true);			// IðÚ(¨÷)
	m_pUIObj[SELECT_UI]->SetActive(true);			// IðÚ(UI)
}

/*
	@brief fÌú»
	@details fÌÀWð¶¬·é
*/
void SceneSelect::InitModel()
{
	SCENEOBJECTINFO obj;		// îñi[p
	CreateObj<ConstantBuffer>("CBLightPower");
	
	for (int i = 0; i < SELECTOBJ_MAX; ++i)
	{
		obj.name = g_szObjectName[i];	// IuWFNg¼O
		obj.pos = g_ObjectPos[i];		// ÀW
		obj.rot = g_ObjectRot[i];		// ñ]
		obj.size = g_ObjectSize[i];		// TCY
		obj.vs = g_vShader[i];			// ¸_VF[_[
		obj.ps = g_pShader[i];			// sNZVF[_[
		
		// îñÌZbg
		m_ObjInitData.push_back(obj);
	}
}

// fÌ¶¬
void SceneSelect::CreateModel()
{
	Model* pModel;		// îñi[p
	for (int i = 0; i < SELECTOBJ_MAX; ++i)
	{
		pModel = CreateObj<Model>(g_szObjectName[i]);
		m_ObjInitData[i].pModel = pModel;
		m_pObjData.push_back(pModel);
	}
}

// Â«õÌú»
void SceneSelect::InitLight()
{
	m_pSceneLight = new CSelectSceneLight();
}

// UIÌúÝè
UIINFO SetUpUI(UIINFO info)
{
	UIINFO pData;
	pData.pos = info.pos;			// ÀW
	pData.size = info.size;			// TCY
	pData.color = info.color;		// F
	pData.kind = info.kind;			// íÞ
	return pData;
}

void SceneSelect::UpdateKeyInput()
{
	// CgÏX(ÊíóÔ)
	if (IsKeyPress(KEY_CHANGELIGHT_NORMAL))
	{
		m_pSceneLight->ChangeLight(LIGHT_NORMAL);
	}
	// CgÏX(¨à)
	else if (IsKeyPress(KEY_CHANGELIGHT_INSIDE))
	{
		m_pSceneLight->ChangeLight(LIGHT_INSIDE);
	}
	// CgÌÏX(é)
	else if (IsKeyPress(KEY_CHANGELIGHT_NIGHT))
	{
		m_pSceneLight->ChangeLight(LIGHT_NIGHT);
	}
}

// 3DÌú»
void SceneSelect::Init3D()
{
	// fÌú»
	InitModel();
	// fÌ¶¬
	CreateModel();
	// IuWFNgÌ¶¬
	CreateObject();
}

// IuWFNgÌ¶¬
void SceneSelect::CreateObject()
{
	// ¨÷IuWFNg
	g_pMeatObj = new CMeat_Setting(m_ObjInitData[SELECTOBJ_MEAT]);
	g_pBasicFig = new CBasicFigure(m_ObjInitData[SELECTOBJ_DEBUGSPHERE], BASICFIGURE_SPHERE);
}

// VF[_[ÌÝè
void SceneSelect::SetShader(SELECTSCENE_OBJECT kind)
{
	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pDirectionalLightCB = GetObj<ConstantBuffer>("CBDirectionalLight");

	// ---- ¸_VF[_[ÌÝè ----
	XMMATRIX mWorld;		// [hÀW
	XMFLOAT4X4 mat[3];

	// [hÏ·
	// ñ]
	mWorld = XMMatrixRotationX(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.x));
	mWorld *= XMMatrixRotationY(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.y));
	mWorld *= XMMatrixRotationZ(XMConvertToRadians(m_pObjData[kind]->m_vertex.rot.z));

	// ÀWÏ·
	mWorld *= XMMatrixTranslation(m_pObjData[kind]->m_vertex.pos.x, m_pObjData[kind]->m_vertex.pos.y, m_pObjData[kind]->m_vertex.pos.z);
	XMStoreFloat4x4(&mat[0], XMMatrixTranspose(mWorld));

	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	pWVP->Write(mat);
	pWVP->BindVS(0);
	// --------------------------

	// ---- sNZVF[_[ÌÝè ----
	XMFLOAT3 dir = m_pSceneLight->GetDir();
	XMFLOAT4 color = m_pSceneLight->GetColor();
	XMFLOAT4 light[] = {
		{dir.x, dir.y, dir.z, 0.0f},
		color
	};
	pDirectionalLightCB->Write(&light);
	pDirectionalLightCB->BindPS(0);
	// --------------------------
}
