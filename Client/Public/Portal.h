#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CPortal final : public CGameObject
{
private:
	explicit CPortal(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_bPlayerTouch() { return m_bPlayerTouch; }
	_vector Get_LinkedPortalPos() { return m_vLinkedPortalPos; }

private:	
	CCollider*			m_pSphereCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	vector<CModel*>		m_Models;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

private:
	_uint				m_ModelIndex = 0;
	_bool				m_bPlayerTouch = false;
	_vector				m_vLinkedPortalPos = XMVectorSet(40.f,0.f,20.f,1.f);

public:
	static CPortal* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END