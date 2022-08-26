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
	
	for (auto& iter : m_Models) {
		m_NameTag.push_back(iter->Get_FileName());
	}

	if (CImgui_Manager::GetInstance()->m_ObjTagList.size() == 0) {
		for (auto& iter : m_NameTag) {
			CImgui_Manager::Item item;
			strcpy_s(item.Name, iter);
			item.is_Selected = false;
			CImgui_Manager::GetInstance()->m_ObjTagList.push_back(item);
		}
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-50.f, 0.f, -50.f, 0.f));
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

	if (nullptr != m_pRendererCom && m_Culling == true)
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
	if(CImgui_Manager::GetInstance()->m_Navimode == false)
		m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CAllObject::Using_Stage1()
{
	CModel* ComModel = nullptr;

	if (FAILED(__super::SetUp_Components(TEXT("Model0"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Grass"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SnowGrass"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Snow_Tree_Pine_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model3"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree_Pine_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model4"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dead_HeightTree"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model5"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dead_Tree_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model6"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dead_Tree_04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model7"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dead_Tree_Snow04"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model8"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);

	if (FAILED(__super::SetUp_Components(TEXT("Model9"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence_02"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model10"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence_03"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model12"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StartHouse"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model13"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StationWagon_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model14"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StationWagon_Sedan_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model15"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Church"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model16"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model17"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StationWagon_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model18"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StationWagon_Sedan_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model19"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Church"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model20"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_UtillityPole_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model21"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_UtillityPole_01_Broken"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model22"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Silo"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model23"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Silo2"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model24"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_WaterTank"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model25"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_Windmill"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model26"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_HayBale_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model27"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree_Fallen_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model28"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BurgerHouse"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model29"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PuzzleMap"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model30"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BossMapObj"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model31"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_GrassStack"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model32"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Farm_HayBale_Stack_01"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model33"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bridge_FencRope"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model34"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence1"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model35"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fence2"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model36"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bridge_Rope"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model37"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Merchant"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model38"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Merchant_Bridge"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);
	if (FAILED(__super::SetUp_Components(TEXT("Model39"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Merchant_Emissive"), (CComponent**)&ComModel)))
		return E_FAIL;
	m_Models.push_back(ComModel);


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
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	ColliderDesc.fRadius = 1.5f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_Sphere"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;
		
	if (FAILED(Using_Stage1()))
		return E_FAIL;

	// LEVEL_GAMEPLAY -> Current LevelIndex·Î
	
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
