#pragma once

#include "Base.h"

/*1. 내 게임에 사용될 객체(사본)들을 보관하고 관리한다. */
/*2. 원형객체들을 보관한다. */
/*3. 사본객체들의 갱신을 담당한다.(Tick함수호출) */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	CGameObject* Add_GameObjToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);

	void Tick(_double TimeDelta);
	void LateTick(_double TimeDelta);
	HRESULT Clear(_uint iLevelIndex);
	class CTransform* Get_Transform(_uint iLevelIndex, const _tchar* pLayerTag, _uint iNumIndex = 0);

public:
	class CGameObject*	Get_Picking_Object(_uint iLevelIndex, const _tchar* pLayerTag);
	list<class CGameObject*>* Get_GameObjectList(_uint iLevelIndex, const _tchar* pLayerTag);


private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private:
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;

private:
	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END