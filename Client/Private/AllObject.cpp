#include "stdafx.h"
#include "..\Public\AllObject.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"

CAllObject::CAllObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CAllObject::CAllObject(const CAllObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAllObject::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAllObject::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	if (pArg != nullptr) {
		memcpy(&m_ModelIndex, pArg, sizeof(int));
	}
	
	for (auto* iter : m_Models[m_LevelIndex]) {
		m_NameTag.push_back(iter->Get_FileName());
	}

	if (CImgui_Manager::GetInstance()->m_ObjListBox.size() == 0) {
		for (auto& iter : m_NameTag) {
			CImgui_Manager::Item item;
			strcpy_s(item.Name, iter);
			item.is_Selected = false;
			CImgui_Manager::GetInstance()->m_ObjListBox.push_back(item);
		}
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-5.f, 0.f, -5.f, 0.f));
	}

	return S_OK;
}

_int CAllObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_Dead == true)
		return 1;

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

void CAllObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	}
}

HRESULT CAllObject::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_Models[m_ModelIndex])
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMeshContainers = m_Models[m_ModelIndex]->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_Models[m_ModelIndex]->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_Models[m_ModelIndex]->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}	

#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CAllObject::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.0f, 0.f);
	ColliderDesc.fRadius = 3.f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Sphere"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;
	
	CModel* ComModel = nullptr;

	// LEVEL_GAMEPLAY -> Current LevelIndex·Î
	
	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_GarbageCan"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GarbageCan"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

#pragma region Road
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_CampgroundRoad"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CampgroundRoad"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_ParkingRoad"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ParkingRoad"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Road0"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Road0"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Road1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Road1"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Road2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Road2"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Road3"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Road3"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
#pragma endregion Road
#pragma region Tree
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Birch_Tree_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Birch_Tree_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Birch_Tree_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Birch_Tree_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Birch_Tree_03"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Birch_Tree_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Birch_Tree_04"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Birch_Tree_04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Birch_Tree_05"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Birch_Tree_05"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dead_Tree_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dead_Tree_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dead_Tree_03"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dead_Tree_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dynamic_Tree_04"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamic_Tree_04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dynamic_Tree_05"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamic_Tree_05"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dynamic_Tree_06"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamic_Tree_06"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dynamic_Tree_07"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamic_Tree_07"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Dynamic_Tree_07b"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dynamic_Tree_07b"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Fallen_Tree_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fallen_Tree_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Fallen_Tree_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fallen_Tree_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Tree_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Tree_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Tree_06"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree_06"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Tree_07b"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree_07b"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
#pragma endregion Tree
#pragma region Rock
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Debris_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Debris_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Debris_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Debris_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Debris_03"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Debris_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Debris_04"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Debris_04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Debris_05"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Debris_05"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Debris_06"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Debris_06"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Pile_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Pile_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Pile_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Pile_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Rock_Pile_03"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rock_Pile_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
#pragma endregion Rock
#pragma region LandScape
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Tubing_Landscape"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tubing_Landscape"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Tubing_Water"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Landscape_Water"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_House0"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House_00"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_House1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_House2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_House3"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House_04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_MailBox02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MailBox"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);


#pragma endregion LandScape
#pragma region 8.11New
	if (FAILED(__super::SetUp_Components(TEXT("Com_BoatHouse"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BoatHouse"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Com_BoatHouse_Plane"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BoatHouse_Plane"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Car_Flat"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Car_Flat"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Car_Sedan"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Car_Sedan"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Car_StationWagon_Sedan"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Car_StationWagon_Sedan"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("ChurchMap"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ChurchMap"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("_DinerMap"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DinerMap"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Fence_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Fence_02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Fence_03"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("GuardRail"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GuardRail"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Aloe_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aloe_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Aloe_03"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aloe_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Aloe_04"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Aloe_04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Camp_Grass"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CampGround_Grass"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Grass_Dynamic"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Grass_Dynamic"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Sign_Walking"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sign_Walking"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Farm_Silo"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Silo"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Farm_Silo2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Silo2"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("WaterTank"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_WaterTank"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Windmill"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Windmill"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("HayBale_01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HayBale_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("HayBale_Stack"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HayBale_Stack_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Farm_Road"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Road"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Tracter"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tracter"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Town_Tree"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Town_Tree"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("UtillityPole01"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_UtillityPole_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("UtillityPole02"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_UtillityPole_Broken"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
#pragma endregion 8.11New
#pragma region Bike
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Bicycle0"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bicycle0"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Bicycle1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bicycle1"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model_Bike"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bike"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
#pragma endregion Bike


	return S_OK;
}

HRESULT CAllObject::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CAllObject * CAllObject::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CAllObject*	pInstance = new CAllObject(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CAllObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAllObject::Clone(void * pArg)
{
	CAllObject*	pInstance = new CAllObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CAllObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAllObject::Free()
{
	__super::Free();	
	
	for (auto* Models : m_Models) {
		Safe_Release(Models);
	}

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);

}
