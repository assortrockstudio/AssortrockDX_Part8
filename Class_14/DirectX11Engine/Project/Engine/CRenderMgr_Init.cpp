#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "assets.h"

#include "CMRT.h"

void CRenderMgr::init()
{
	// RenderTarget �� ������� �� �ִ� �ؽ��ĸ� ����
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_RenderTargetCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetCopyTex"
		, (UINT)vRenderResol.x, (UINT)vRenderResol.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);


	// MRT ����
	CreateMRT();
}

void CRenderMgr::CreateMRT()
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	CMRT* pMRT = nullptr;

	// =============
	// SwapChain MRT
	// =============	
	pMRT = m_MRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

	Ptr<CTexture> RTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");

	// DepthStencil Texture �� �����Ѵ�.
	Ptr<CTexture> DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex"
								, (UINT)vResolution.x, (UINT)vResolution.y
								, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	pMRT->Create(&RTTex, 1, DSTex);

	


}