#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_Info{}
	, m_LightIdx(-1)
{
}

CLight3D::~CLight3D()
{
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.LightType = (UINT)_Type;
}

void CLight3D::finaltick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}