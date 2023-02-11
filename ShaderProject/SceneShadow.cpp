#include "SceneShadow.h"
#include "Model.h"
#include "TextureFactory.h"
#include "ShaderList.h"
#include "ConstantBuffer.h"
#include "CameraBase.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "CBlendState.h"

void SceneShadow::Init()
{
	// 地面のモデル
	Model* pModel = CreateObj<Model>("Ground");
	pModel->Load("Assets/Model/model_moutain_02.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
	pModel->SetPixelShader(GetPS(PS_DEPTHSHADOW));
	/*pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_LAMBERT));*/

	// 影描画のための書き込み先を作成する
	Texture* pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT, 1280.0f, 720.0f);
	RegisterObj<Texture>("ShadowCanvas", pCanvas);

	DepthStencil* pDepth = reinterpret_cast<DepthStencil*>(TextureFactory::CreateDepthStencil(1280.0f, 720.0f, true));
	RegisterObj<DepthStencil>("ShadowDSV", pDepth);

	// 太陽の行列を作成
	ConstantBuffer* pSunBuf = CreateObj<ConstantBuffer>("SunBuf");
	pSunBuf->Create(sizeof(DirectX::XMFLOAT4X4) * 2);
}
void SceneShadow::Uninit()
{
}
void SceneShadow::Update()
{
}
void SceneShadow::Draw()
{
	CBlendState::SetZBuffer(true);

	CameraBase* pCamera = GetObj<CameraBase>("CameraDCC");
	ConstantBuffer* pWVP = GetObj<ConstantBuffer>("CBWVP");
	ConstantBuffer* pLight = GetObj<ConstantBuffer>("CBLight");
	Model* pPlayerModel = GetObj<Model>("UnityModel");
	Model* pGroundModel = GetObj<Model>("Ground");

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());

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
		DirectX::XMVectorSet(3.0f, 3.0f, -3.0f, 0.0f),
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
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(sunProj));
	pWVP->Write(mat);
	pWVP->BindVS(0);

	// 影になるオブヘクトを描画
	// 部屋の床に、Unitychanの影を落とすので、Unitychanだけ書き込む
	// ※部屋も書き込むと、部屋の天井の影が書き込まれてUnitychanの影が見えなくなる
	pPlayerModel->SetVertexShader(GetVS(VS_DEPTHWRITE));
	pPlayerModel->SetPixelShader(GetPS(PS_DEPTHWRITE));
	pPlayerModel->Draw();

	// 描画先を元に戻す
	RenderTarget* pRTV = GetObj<RenderTarget>("DefRTV");
	pView = pRTV->GetView();
	pDepth = GetObj<DepthStencil>("DefDSV");
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// ゲーム画面の描画
	ConstantBuffer* pSunBuf = GetObj<ConstantBuffer>("SunBuf");
	pSunBuf->Write(&mat[1]);
	pSunBuf->BindVS(1);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();
	pWVP->Write(mat);
	pWVP->BindVS(0);

	DirectX::XMFLOAT4 lightDir(-1.0f, -1.0f, 1.0f, 0.0f);
	pLight->Write(&lightDir);
	pLight->BindPS(0);

	SetTexturePS(pCanvas->GetResource(), 1);

	CBlendState::SetBlendState(BS_NONE);

	// モデル描画
	pPlayerModel->SetVertexShader(GetVS(VS_OBJECT));
	pPlayerModel->SetPixelShader(GetPS(PS_SHADOWRECIEVER));
	pPlayerModel->Draw();

	// 地面描画
	pGroundModel->Draw();
}

