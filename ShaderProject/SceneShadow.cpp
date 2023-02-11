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
	// �n�ʂ̃��f��
	Model* pModel = CreateObj<Model>("Ground");
	pModel->Load("Assets/Model/model_moutain_02.fbx", 0.005f);
	pModel->SetVertexShader(GetVS(VS_GROUNDSHADOW));
	pModel->SetPixelShader(GetPS(PS_DEPTHSHADOW));
	/*pModel->SetVertexShader(GetVS(VS_OBJECT));
	pModel->SetPixelShader(GetPS(PS_LAMBERT));*/

	// �e�`��̂��߂̏������ݐ���쐬����
	Texture* pCanvas = TextureFactory::CreateRenderTarget(DXGI_FORMAT_R32_FLOAT, 1280.0f, 720.0f);
	RegisterObj<Texture>("ShadowCanvas", pCanvas);

	DepthStencil* pDepth = reinterpret_cast<DepthStencil*>(TextureFactory::CreateDepthStencil(1280.0f, 720.0f, true));
	RegisterObj<DepthStencil>("ShadowDSV", pDepth);

	// ���z�̍s����쐬
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
		DirectX::XMVectorSet(3.0f, 3.0f, -3.0f, 0.0f),
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
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(sunView));
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(sunProj));
	pWVP->Write(mat);
	pWVP->BindVS(0);

	// �e�ɂȂ�I�u�w�N�g��`��
	// �����̏��ɁAUnitychan�̉e�𗎂Ƃ��̂ŁAUnitychan������������
	// ���������������ނƁA�����̓V��̉e���������܂��Unitychan�̉e�������Ȃ��Ȃ�
	pPlayerModel->SetVertexShader(GetVS(VS_DEPTHWRITE));
	pPlayerModel->SetPixelShader(GetPS(PS_DEPTHWRITE));
	pPlayerModel->Draw();

	// �`�������ɖ߂�
	RenderTarget* pRTV = GetObj<RenderTarget>("DefRTV");
	pView = pRTV->GetView();
	pDepth = GetObj<DepthStencil>("DefDSV");
	GetContext()->OMSetRenderTargets(1, &pView, pDepth->GetView());

	// �Q�[����ʂ̕`��
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

	// ���f���`��
	pPlayerModel->SetVertexShader(GetVS(VS_OBJECT));
	pPlayerModel->SetPixelShader(GetPS(PS_SHADOWRECIEVER));
	pPlayerModel->Draw();

	// �n�ʕ`��
	pGroundModel->Draw();
}

