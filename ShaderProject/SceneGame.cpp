#include "SceneGame.h"
#include "ShaderList.h"
#include "CameraBase.h"
#include "TextureFactory.h"
#include "CBlendState.h"
#include <vector>
#include "Input.h"
#include "DepthStencil.h"		// �f�v�X�X�e���V��
#include "RenderTarget.h"		// �����_�[�^�[�Q�b�g
//#include "CPlayer.h"			// �v���C���[�N���X
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

// �V�F�[�_�[�̎��
enum KIND_SHADER
{
	SHADER_LAMBERT = 0,		// �g�U���ˌ��̂�
	SHADER_TOON,			// �g�D�[����
	SHADER_MAX
};

// UI�̎��
enum UI_KIND_GAMESCENE
{
	UI_LOADTEXTURE = 0,		// ���[�h�V�[���̃e�N�X�`��
	UI_HPBAR_BASE,			// HP�o�[�̃x�[�X
	UI_HPBAR_DESIGN,		// HP�o�[�f�U�C������
	UI_GAMESCENE_MAX
};

// ���z��
DirectX::XMFLOAT3 g_lightDir;
// �V�F�[�_�[�̎��
KIND_SHADER g_shaderKind;
std::vector<CUI*> g_vecObjUI;
float g_fMaxHP = 100.0f;
float g_fCurrentHP = g_fMaxHP;
float g_fHPBarMoveSpeed = 0.001f;
DirectX::XMFLOAT2 g_fHPBarUVPos = { 0.0f, 0.0f };
//CNewPlayer* g_pPlayer;

void SceneGame::Init()
{
	// ���z���̏�����
	g_lightDir = { 0.0f, 0.0f, 0.0f };

	// �n�ʂ̃��f��
	Model* pModel = CreateObj<Model>("BGMountain");
	pModel->Load("Assets/Model/model_moutain_02.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
	pModel->SetPixelShader(GetPS(PS_DEPTHSHADOW));
	/*pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_LAMBERT));*/

	// �v���C���[
	//g_pPlayer = new CNewPlayer();
	//g_pPlayer->Init(GetObj<Model>("UnityModel"));

	// �e�`��̂��߂̏������ݐ���쐬����
	Texture* pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT, 1280.0f, 720.0f);
	RegisterObj<Texture>("ShadowCanvas", pCanvas);

	DepthStencil* pDepth = reinterpret_cast<DepthStencil*>(TextureFactory::CreateDepthStencil(1280.0f, 720.0f, true));
	RegisterObj<DepthStencil>("ShadowDSV", pDepth);

	// ���z�̍s����쐬
	ConstantBuffer* pSunBuf = CreateObj<ConstantBuffer>("SunBuf");
	pSunBuf->Create(sizeof(DirectX::XMFLOAT4X4) * 2);

	// �g�D�[���V�F�[�_�[�̐�
	ConstantBuffer* pToonLineCB = CreateObj<ConstantBuffer>("ToonLineCB");		// �g�D�[���V�F�[�_�[�̐��̏��
	pToonLineCB->Create(sizeof(DirectX::XMFLOAT4));
	// �g�D�[���e�N�X�`���̏�����
	Texture* pTex = TextureFactory::CreateFromFile("Assets/ramp.png");
	RegisterObj<Texture>("RampTex", pTex);

	// �n�߂Ƀ��[�h���o�����邩 true������� 
	m_isLoad = false;

	// �n�߂̃V�F�[�_�[�̎��
	g_shaderKind = SHADER_LAMBERT;
}

void SceneGame::Uninit()
{
}

void SceneGame::Update()
{
	if (IsKeyPress(VK_SPACE))
		m_isLoad = false;

	// ���[�h���̓e�N�X�`�����΂ߏ�Ɉړ�����
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
		// ���z���̈ʒu��ύX����
		float lightPos[3] = { g_lightDir.x, g_lightDir.y , g_lightDir.z };
		ImGui::SliderFloat3("LightDir", lightPos, -1.0f, 1.0f);
		g_lightDir.x = lightPos[0];
		g_lightDir.y = lightPos[1];
		g_lightDir.z = lightPos[2];

		// ���f���̃V�F�[�_�[��ύX����
		if (ImGui::Button("Toon"))
			g_shaderKind = SHADER_TOON;
		else if (ImGui::Button("Lambert"))
			g_shaderKind = SHADER_LAMBERT;
		ImGui::SliderFloat("HP", &g_fCurrentHP, 0.0f, g_fMaxHP);

		// HP�o�[�̃f�U�C���𓮂���
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

	// �e��`�悷��
	DrawShadow(pPlayerModel, mat, pWVP);

	// �Q�[����ʂ̕`��
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

	// 3D���f����
	if (g_shaderKind == SHADER_LAMBERT)
	{
		SetTexturePS(GetObj<Texture>("ShadowCanvas")->GetResource(), 1);
		// ���f���`��
		pPlayerModel->SetVertexShader(GetVS(VS_OBJECT));
		pPlayerModel->SetPixelShader(GetPS(PS_SHADOWRECIEVER));
		pPlayerModel->Draw();

		// �n�ʕ`��
		pGroundModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
		pGroundModel->SetPixelShader(GetPS(PS_DEPTHSHADOW));
		pGroundModel->Draw();
	}
	// �g�D�[����
	else if (g_shaderKind == SHADER_TOON)
	{
		SetTexturePS(GetObj<Texture>("RampTex")->GetResource(), 1);
		// === �K���̉e��\�� ===
		SetCullingMode(D3D11_CULL_NONE);
		// --- �v���C���[ ---
		pPlayerModel->SetVertexShader(GetVS(VS_OBJECT));
		//pModel->SetPixelShader(GetPS(PS_TOON));
		pPlayerModel->SetPixelShader(GetPS(PS_TOON));
		pPlayerModel->Draw();
		// -----------------

		SetTexturePS(GetObj<Texture>("ShadowCanvas")->GetResource(), 2);
		// --- �w�i ----------
		pGroundModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
		pGroundModel->SetPixelShader(GetPS(PS_DEPTHSHADOW_TOON));
		pGroundModel->Draw();
		// ====================

		// === �֊s����\�� ===
		SetCullingMode(D3D11_CULL_BACK);
		// --- �v���C���[ ---
		// ���̑���
		DirectX::XMFLOAT4 ToonLine(0.005f, 0.0f, 0.0f, 0.0f);
		pToonLineCB->Write(&ToonLine);
		pToonLineCB->BindVS(1);
		// �`��
		pPlayerModel->SetVertexShader(GetVS(VS_OUTLINE));
		pPlayerModel->SetPixelShader(GetPS(PS_OUTLINE));
		pPlayerModel->Draw();
		// ------------------
		// --- �w�i ---
		// ���̑���
		ToonLine = { 0.05f, 0.0f, 0.0f, 0.0f };
		pToonLineCB->Write(&ToonLine);
		pToonLineCB->BindVS(1);
		// �`��
		pGroundModel->SetVertexShader(GetVS(VS_OUTLINE));
		pGroundModel->SetPixelShader(GetPS(PS_OUTLINE));
		pGroundModel->Draw();
		// --------------------
		// ====================

		SetCullingMode(D3D11_CULL_NONE);
	}
	
}

// �e��`�悷��
void SceneGame::DrawShadow(Model* pModel, XMFLOAT4X4 matrix[], ConstantBuffer* pCB)
{
	// �e�`��̂��߂ɏ������ݐ��ύX����
	Texture* pCanvas = GetObj<Texture>("ShadowCanvas");
	DepthStencil* pDepth = GetObj<DepthStencil>("ShadowDSV");
	RenderTarget* pRTVCanvas = reinterpret_cast<RenderTarget*>(pCanvas);
	ID3D11RenderTargetView* pView = pRTVCanvas->GetView();
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// �`���̃N���A
	// ���s�̔�r�̂��߂�1�ԉ���1�������l�Őݒ�
	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
	GetContext()->ClearRenderTargetView(pView, color);
	GetContext()->ClearDepthStencilView(pDepth->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ���z�̈ʒu�ɃJ����������Ƒz�肵�āA�e�N�X�`���ɏ�������
	//���J�������߂����āA�e�N�X�`���ɏ������߂Ȃ��������o�Ă���ƁA�������\������Ȃ��̂ŃJ�����𗣂��Ĉʒu��ݒ肷��
	DirectX::XMMATRIX sunView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(3.0f * (g_lightDir.x + 0.1f) , 3.0f * (g_lightDir.y + 0.1f), -3.0f * (g_lightDir.z + 0.1f), 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	DirectX::XMMATRIX sunProj = DirectX::XMMatrixOrthographicLH(
		5.0f, 5.0f * (9.0f / 16.0f), 0.2f, 100.0f
	);
	// ���z�̌��͂قڂ܂��������ł���̂ŁA�p�x�����������e�ł͐������Ȃ�
	//	DirectX::XMMatrixPerspectiveFovLH(
	//	DirectX::XMConvertToRadians(60.0f), 16.0f / 9.0f, 0.2f, 100.0f
	//);
	DirectX::XMStoreFloat4x4(&matrix[1], DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&matrix[2], DirectX::XMMatrixTranspose(sunProj));
	pCB->Write(matrix);
	pCB->BindVS(0);

	// �e�ɂȂ�I�u�w�N�g��`��
	// �����̏��ɁAUnitychan�̉e�𗎂Ƃ��̂ŁAUnitychan������������
	// ���������������ނƁA�����̓V��̉e���������܂��Unitychan�̉e�������Ȃ��Ȃ�
	pModel->SetVertexShader(GetVS(VS_DEPTHWRITE));
	pModel->SetPixelShader(GetPS(PS_DEPTHWRITE));
	pModel->Draw();

	// �`�������ɖ߂�
	RenderTarget* pRTV = GetObj<RenderTarget>("DefRTV");
	pView = pRTV->GetView();
	pDepth = GetObj<DepthStencil>("DefDSV");
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());
}



