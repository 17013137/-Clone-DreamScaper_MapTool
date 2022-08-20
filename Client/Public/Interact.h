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

class CInteract final : public CGameObject
{
	enum STAGE{ STAGE1, STAGE2, STAGE3, STAGE_END };

private:
	explicit CInteract(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CInteract(const CInteract& rhs);
	virtual ~CInteract() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_ModelINdex(_uint flag) { m_ModelIndex = flag; }
	const _uint& Get_ModelINdex() { return m_ModelIndex; }

private:	
	CCollider*			m_pSphereCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	
private:
	HRESULT Using_Stage1();
	//HRESULT Using_Stage2();
	//HRESULT Using_Stage3();

private:
	vector<CModel*> m_Models;
	vector<char*> m_NameTag;

private:
	_int m_LevelIndex = 0;
	_uint m_ModelIndex = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();	

public:
	static CInteract* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END