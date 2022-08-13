#include "stdafx.h"
#include "..\Public\Stage_Manager.h"
#include "GameInstance.h"
#include "Portal.h"

CStage_Manager*	CStage_Manager::m_pInstance = nullptr;

_vector CStage_Manager::Player_FPressing()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform*		pTargetTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));

	for (auto iter = Portal_List.begin(); iter != Portal_List.end(); ++iter) {
		CPortal* Portal = dynamic_cast<CPortal*>(*iter);
		if (Portal->Get_bPlayerTouch()) {
			RELEASE_INSTANCE(CGameInstance);
			_vector LinkedPos = Portal->Get_LinkedPortalPos();
			return LinkedPos;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	return XMVectorSet(0.f,0.f,0.f,-1.f);
}

CStage_Manager::CStage_Manager()
{
}


CStage_Manager::~CStage_Manager()
{
	Free();
}



void CStage_Manager::Free()
{
}
