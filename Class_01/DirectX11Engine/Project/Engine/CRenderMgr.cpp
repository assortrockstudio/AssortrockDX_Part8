#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CAssetMgr.h"

#include "CCamera.h"
#include "CLight2D.h"

#include "CLevelMgr.h"

CRenderMgr::CRenderMgr()
	: m_EditorCam(nullptr)
	, m_Light2DBuffer(nullptr)	
{
	m_Light2DBuffer = new CStructuredBuffer;
	Render_Func = &CRenderMgr::render_play;
}

CRenderMgr::~CRenderMgr()
{
	delete m_Light2DBuffer;
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRenderTargetTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_RenderTargetCopyTex->GetTex2D().Get(), pRenderTargetTex->GetTex2D().Get());
}

void CRenderMgr::init()
{
	// RenderTarget �� ������� �� �ִ� �ؽ��ĸ� ����
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_RenderTargetCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetCopyTex"
												, (UINT)vRenderResol.x, (UINT)vRenderResol.y
												, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
}

void CRenderMgr::tick()
{
}

void CRenderMgr::render()
{
	if (!CLevelMgr::GetInst()->GetCurrentLevel())
		return;

	// SwapChain �� ����۸� ����Ÿ������ ������
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());
	
	// �������� �ʿ��� ������ ���ε�
	DataBinding();

	// Target Clear
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	(this->*Render_Func)();

	// ������ Ŭ����
	DataClear();
}

void CRenderMgr::render_play()
{
	// �� ī�޶� �����ִ� ����� ����Ÿ�ٿ� �׸���.
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_editor()
{
	if(nullptr != m_EditorCam)
		m_EditorCam->render();
}

void CRenderMgr::DataBinding()
{
	// GlobalData ���ε�
	g_GlobalData.Light2DCount = (int)m_vecLight2D.size();
	//g_GlobalData.Light3DCount = (int)m_vecLight3D.size();

	static CConstBuffer* GlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	GlobalBuffer->SetData(&g_GlobalData);
	GlobalBuffer->Binding();
	GlobalBuffer->Binding_CS();


	// ���� �������� ����ȭ���� ���ũ�Ⱑ �� ������ Ȯ���Ѵ�.
	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);
	}

	// m_vecLight2D �� ���� ������ ������ ����ȭ���۷� ������.
	static vector<tLightInfo> vecLightInfo;
	vecLightInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	// ������ ������ 1�� �̻� �ִٸ�
	if (!vecLightInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLightInfo.data(), m_vecLight2D.size());
		m_Light2DBuffer->Binding(15);
	}
}

void CRenderMgr::DataClear()
{
	m_vecLight2D.clear();
}


void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Priority)
{
	// ��ϵ� �ε��� ��ġ��ŭ ���Ͱ� Ȯ��� ��Ȳ�� �ƴϸ� ���͸� �ø���.
	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	if (m_vecCam[_Priority] && m_vecCam[_Priority] != _Cam)
		assert(nullptr);

	m_vecCam[_Priority] = _Cam;
}