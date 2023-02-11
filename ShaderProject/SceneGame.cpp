#include "SceneGame.h"
#include "ShaderList.h"
#include "CameraBase.h"
#include "TextureFactory.h"
#include "CBlendState.h"
#include <vector>
#include "Input.h"
#include "DepthStencil.h"		// デプスステンシル
#include "RenderTarget.h"		// レンダーターゲット
//#include "CPlayer.h"			// プレイヤークラス
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

// シェーダーの種類
enum KIND_SHADER
{
	SHADER_LAMBERT = 0,		// 拡散反射光のみ
	SHADER_TOON,			// トゥーン調
	SHADER_MAX
};

// UIの種類
enum UI_KIND_GAMESCENE
{
	UI_LOADTEXTURE = 0,		// ロードシーンのテクスチャ
	UI_HPBAR_BASE,			// HPバーのベース
	UI_HPBAR_DESIGN,		// HPバーデザイン部分
	UI_GAMESCENE_MAX
};

// 太陽光
DirectX::XMFLOAT3 g_lightDir;
// シェーダーの種類
KIND_SHADER g_shaderKind;
std::vector<CUI*> g_vecObjUI;
float g_fMaxHP = 100.0f;
float g_fCurrentHP = g_fMaxHP;
float g_fHPBarMoveSpeed = 0.001f;
DirectX::XMFLOAT2 g_fHPBarUVPos = { 0.0f, 0.0f };
//CNewPlayer* g_pPlayer;

void SceneGame::Init()
{
	// 太陽光の初期化
	g_lightDir = { 0.0f, 0.0f, 0.0f };

	// 地面のモデル
	Model* pModel = CreateObj<Model>("BGMountain");
	pModel->Load("Assets/Model/model_moutain_02.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
	pModel->SetPixelShader(GetPS(PS_DEPTHSHADOW));
	/*pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_LAMBERT));*/

	// プレイヤー
	//g_pPlayer = new CNewPlayer();
	//g_pPlayer->Init(GetObj<Model>("UnityModel"));

	// 影描画のための書き込み先を作成する
	Texture* pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT, 1280.0f, 720.0f);
	RegisterObj<Texture>("ShadowCanvas", pCanvas);

	DepthStencil* pDepth = reinterpret_cast<DepthStencil*>(TextureFactory::CreateDepthStencil(1280.0f, 720.0f, true));
	RegisterObj<DepthStencil>("ShadowDSV", pDepth);

	// 太陽の行列を作成
	ConstantBuffer* pSunBuf = CreateObj<ConstantBuffer>("SunBuf");
	pSunBuf->Create(sizeof(DirectX::XMFLOAT4X4) * 2);

	// トゥーンシェーダーの線
	ConstantBuffer* pToonLineCB = CreateObj<ConstantBuffer>("ToonLineCB");		// トゥーンシェーダーの線の情報
	pToonLineCB->Create(sizeof(DirectX::XMFLOAT4));
	// トゥーンテクスチャの初期化
	Texture* pTex = TextureFactory::CreateFromFile("Assets/ramp.png");
	RegisterObj<Texture>("RampTex", pTex);

	// 始めにロード演出を入れるか true→入れる 
	m_isLoad = false;

	// 始めのシェーダーの種類
	g_shaderKind = SHADER_LAMBERT;
}

void SceneGame::Uninit()
{
}

void SceneGame::Update()
{
	if (IsKeyPress(VK_SPACE))
		m_isLoad = false;

	// ロード中はテクスチャを斜め上に移動する
	if (m_isLoad)
	{
		g_fHPBarUVPos.x += g_fHPBarMoveSpeed;
		g_fHPBarUVPos.y += g_fHPBarMoveSpeed;

		if (g_fHPBarUVPos.x > 1.0f)
		{
			g_fHPBarUVPos.x = 0.0f;
		}

		if (g_fHPBarUVPos.y > 1.0f)
		{
			g_fHPBarUVPos.y = 0.0f;
		}
	}
	else
	{
		Model* pModel;
		// 太陽光の位置を変更する
		float lightPos[3] = { g_lightDir.x, g_lightDir.y , g_lightDir.z };
		ImGui::SliderFloat3("LightDir", lightPos, -1.0f, 1.0f);
		g_lightDir.x = lightPos[0];
		g_lightDir.y = lightPos[1];
		g_lightDir.z = lightPos[2];

		// モデルのシェーダーを変更する
		if (ImGui::Button("Toon"))
			g_shaderKind = SHADER_TOON;
		else if (ImGui::Button("Lambert"))
			g_shaderKind = SHADER_LAMBERT;
		ImGui::SliderFloat("HP", &g_fCurrentHP, 0.0f, g_fMaxHP);

		// HPバーのデザインを動かす
		g_fHPBarUVPos.x += g_fHPBarMoveSpeed;

		if (g_fHPBarUVPos.x > 1.0f)
		{
			g_fHPBarUVPos.x = 0.0f;
		}
	}
}

void SceneGame::Draw()
{
	DirectX::XMFLOAT4 info;

	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	ConstantBuffer* pToonLineCB = GetObj<ConstantBuffer>("ToonLineCB");
	Model* pPlayerModel = GetObj<Model>("UnityModel");
	Model* pGroundModel = GetObj<Model>("BGMountain");

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());

	// 影を描画する
	DrawShadow(pPlayerModel, mat, pWVP);

	// ゲーム画面の描画
	ConstantBuffer* pSunBuf = GetObj<ConstantBuffer>("SunBuf");
	pSunBuf->Write(&mat[1]);
	pSunBuf->BindVS(1);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	pWVP->Write(mat);
	pWVP->BindVS(0);

	DirectX::XMFLOAT4 lightDir(g_lightDir.x, g_lightDir.y, g_lightDir.z, 0.0f);
	//DirectX::XMFLOAT4 lightDir(1.0f, 1.0f, 0.0f, 0.0f);
	pLight->Write(&lightDir);
	pLight->BindPS(0);

	CBlendState::SetBlendState(BS_NONE);

	// 3Dモデル調
	if (g_shaderKind == SHADER_LAMBERT)
	{
		SetTexturePS(GetObj<Texture>("ShadowCanvas")->GetResource(), 1);
		// モデル描画
		pPlayerModel->SetVertexShader(GetVS(VS_OBJECT));
		pPlayerModel->SetPixelShader(GetPS(PS_SHADOWRECIEVER));
		pPlayerModel->Draw();

		// 地面描画
		pGroundModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
		pGroundModel->SetPixelShader(GetPS(PS_DEPTHSHADOW));
		pGroundModel->Draw();
	}
	// トゥーン調
	else if (g_shaderKind == SHADER_TOON)
	{
		SetTexturePS(GetObj<Texture>("RampTex")->GetResource(), 1);
		// === 階調の影を表示 ===
		SetCullingMode(D3D11_CULL_NONE);
		// --- プレイヤー ---
		pPlayerModel->SetVertexShader(GetVS(VS_OBJECT));
		//pModel->SetPixelShader(GetPS(PS_TOON));
		pPlayerModel->SetPixelShader(GetPS(PS_TOON));
		pPlayerModel->Draw();
		// -----------------

		SetTexturePS(GetObj<Texture>("ShadowCanvas")->GetResource(), 2);
		// --- 背景 ----------
		pGroundModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
		pGroundModel->SetPixelShader(GetPS(PS_DEPTHSHADOW_TOON));
		pGroundModel->Draw();
		// ====================

		// === 輪郭線を表示 ===
		SetCullingMode(D3D11_CULL_BACK);
		// --- プレイヤー ---
		// 線の太さ
		DirectX::XMFLOAT4 ToonLine(0.005f, 0.0f, 0.0f, 0.0f);
		pToonLineCB->Write(&ToonLine);
		pToonLineCB->BindVS(1);
		// 描画
		pPlayerModel->SetVertexShader(GetVS(VS_OUTLINE));
		pPlayerModel->SetPixelShader(GetPS(PS_OUTLINE));
		pPlayerModel->Draw();
		// ------------------
		// --- 背景 ---
		// 線の太さ
		ToonLine = { 0.05f, 0.0f, 0.0f, 0.0f };
		pToonLineCB->Write(&ToonLine);
		pToonLineCB->BindVS(1);
		// 描画
		pGroundModel->SetVertexShader(GetVS(VS_OUTLINE));
		pGroundModel->SetPixelShader(GetPS(PS_OUTLINE));
		pGroundModel->Draw();
		// --------------------
		// ====================

		SetCullingMode(D3D11_CULL_NONE);
	}
	
}

// 影を描画する
void SceneGame::DrawShadow(Model* pModel, XMFLOAT4X4 matrix[], ConstantBuffer* pCB)
{
	// 影描画のために書き込み先を変更する
	Texture* pCanvas = GetObj<Texture>("ShadowCanvas");
	DepthStencil* pDepth = GetObj<DepthStencil>("ShadowDSV");
	RenderTarget* pRTVCanvas = reinterpret_cast<RenderTarget*>(pCanvas);
	ID3D11RenderTargetView* pView = pRTVCanvas->GetView();
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// 描画先のクリア
	// 奥行の比較のために1番遠い1を初期値で設定
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
	GetContext()->ClearRenderTargetView(pView, color);
	GetContext()->ClearDepthStencilView(pDepth->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 太陽の位置にカメラがあると想定して、テクスチャに書き込む
	//※カメラが近すぎて、テクスチャに書き込めない部分が出てくると、正しく表示されないのでカメラを離して位置を設定する
	DirectX::XMMATRIX sunView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(3.0f * (g_lightDir.x + 0.1f) , 3.0f * (g_lightDir.y + 0.1f), -3.0f * (g_lightDir.z + 0.1f), 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	DirectX::XMMATRIX sunProj = DirectX::XMMatrixOrthographicLH(
		5.0f, 5.0f * (9.0f / 16.0f), 0.2f, 100.0f
	);
	// 太陽の光はほぼまっすぐ飛んでいるので、角度がつく透視投影では正しくない
	//	DirectX::XMMatrixPerspectiveFovLH(
	//	DirectX::XMConvertToRadians(60.0f), 16.0f / 9.0f, 0.2f, 100.0f
	//);
	DirectX::XMStoreFloat4x4(&matrix[1], DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&matrix[2], DirectX::XMMatrixTranspose(sunProj));
	pCB->Write(matrix);
	pCB->BindVS(0);

	// 影になるオブヘクトを描画
	// 部屋の床に、Unitychanの影を落とすので、Unitychanだけ書き込む
	// ※部屋も書き込むと、部屋の天井の影が書き込まれてUnitychanの影が見えなくなる
	pModel->SetVertexShader(GetVS(VS_DEPTHWRITE));
	pModel->SetPixelShader(GetPS(PS_DEPTHWRITE));
	pModel->Draw();

	// 描画先を元に戻す
	RenderTarget* pRTV = GetObj<RenderTarget>("DefRTV");
	pView = pRTV->GetView();
	pDepth = GetObj<DepthStencil>("DefDSV");
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());
}



