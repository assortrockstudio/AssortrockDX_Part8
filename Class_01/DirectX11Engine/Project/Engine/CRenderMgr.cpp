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
	// RenderTarget 을 복사받을 수 있는 텍스쳐를 생성
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

	// SwapChain 의 백버퍼를 렌더타겟으로 재지정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());
	
	// 렌더링에 필요한 데이터 바인딩
	DataBinding();

	// Target Clear
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearTarget(ClearColor);

	(this->*Render_Func)();

	// 데이터 클리어
	DataClear();
}

void CRenderMgr::render_play()
{
	// 각 카메라가 보고있는 장면을 렌더타겟에 그린다.
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
	// GlobalData 바인딩
	g_GlobalData.Light2DCount = (int)m_vecLight2D.size();
	//g_GlobalData.Light3DCount = (int)m_vecLight3D.size();

	static CConstBuffer* GlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	GlobalBuffer->SetData(&g_GlobalData);
	GlobalBuffer->Binding();
	GlobalBuffer->Binding_CS();


	// 광원 개수보다 구조화버퍼 요소크기가 더 적으면 확장한다.
	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);
	}

	// m_vecLight2D 에 모인 광원의 정보를 구조화버퍼로 보낸다.
	static vector<tLightInfo> vecLightInfo;
	vecLightInfo.clear();

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLightInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	// 광원이 레벨에 1개 이상 있다면
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
	// 등록될 인덱스 위치만큼 벡터가 확장된 상황이 아니면 벡터를 늘린다.
	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	if (m_vecCam[_Priority] && m_vecCam[_Priority] != _Cam)
		assert(nullptr);

	m_vecCam[_Priority] = _Cam;
}