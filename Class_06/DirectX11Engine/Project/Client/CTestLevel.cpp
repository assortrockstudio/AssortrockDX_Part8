#include "pch.h"

#include "CTestLevel.h"

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAnim2D.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CCameraMoveScript.h>
#include <Scripts/CBackgroundScript.h>
#include <Scripts/CMissileScript.h>
		 
#include <Engine/CCollisionMgr.h>
#include <Engine/CSetColorCS.h>
		 
#include <Engine/CStructuredBuffer.h>
#include <Engine/CPrefab.h>


void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = nullptr;

	// 임시 레벨 설정
	pLevel = new CLevel;
	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"Monster");
		
	// Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->SetCameraPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pLevel->AddObject(0, pCamObj);
	
	// Light3D
	CGameObject* pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight3D);

	pLight->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));	
	pLight->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pLight->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));

	pLevel->AddObject(0, pLight);

	// Player
	CGameObject* Player = new CGameObject;
	Player->SetName(L"Player");

	Player->AddComponent(new CTransform);
	Player->AddComponent(new CMeshRender);
	
	Player->Transform()->SetRelativePos(Vec3(0.f, 0.f, 700.f));
	Player->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));

	Player->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	Player->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3DMtrl"));

	pLevel->AddObject(0, Player);

	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}





void CTestLevel::CreatePrefab()
{
	// Particle Prefab
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"Particle");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pParticleObj->ParticleSystem()->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\particle\\AlphaCircle.png", L"texture\\particle\\AlphaCircle.png"));

	Ptr<CPrefab> Prefab = new CPrefab(pParticleObj);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"ParticlePrefab", Prefab);

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	Prefab->Save(FilePath + L"prefab\\Particle.pref");



	// Missile Prefab 
	CGameObject* pNewObj = new CGameObject;
	pNewObj->SetName(L"Missile");
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);
	pNewObj->AddComponent(new CMissileScript);
	pNewObj->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 40.f));
	pNewObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pNewObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Prefab = new CPrefab(pNewObj);
	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", Prefab);
	Prefab->Save(FilePath + L"prefab\\Missile.pref");
}