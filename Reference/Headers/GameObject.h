#pragma once

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag) {
		return Find_Component(pComponentTag);
	}

	CTransform* Get_Transform() {
		return m_pTransformCom;
	}

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void Set_Dead() { m_Dead = true; }
	void Set_Alpha(_float flag) { m_Alpha = flag; }

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

protected:
	static const _tchar*	m_pTransformTag;
	class CTransform*		m_pTransformCom = nullptr;

protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

protected:
	HRESULT SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

private:
	CComponent* Find_Component(const _tchar* pComponentTag);

protected:
	_uint m_iLevelIndex = 0;
	_bool m_Dead = false;
	_bool m_Culling = false;
	_float m_Alpha = 1.f;
	_float m_Speed = 1.f;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;


};

END