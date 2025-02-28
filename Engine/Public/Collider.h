#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPEWEAPON_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vPosition;
		_float3			vSize;
		_float			fRadius;
	}COLLIDERDESC;

	typedef struct tagObbDesc
	{
		_float3			vCenter;
		_float3			vAlignAxis[3];
		_float3			vCenterAxis[3];
	}OBBDESC;


public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool Collsion_AABB(CCollider* pTargetCollider);
	_bool Collsion_OBB(CCollider* pTargetCollider);
	void Update(_fmatrix WorldMatrix);
	_vector Intersect_Ray();
	void Set_Color(_float3 flag) { m_fColor = flag; }

public:
	void Set_isCollison(_bool flag) { m_isCollision = flag; }

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	TYPE						m_eType = TYPEWEAPON_END;
	_bool						m_isCollision = false;
	BoundingBox*				m_pAABB = nullptr;
	BoundingBox*				m_pAABBInWorld = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;
	BoundingOrientedBox*		m_pOBBInWorld = nullptr;
	BoundingSphere*				m_pSPHERE = nullptr;
	BoundingSphere*				m_pSPHEREInWorld = nullptr;
	
	_float3 m_fColor = _float3(0.f, 1.f, 0.f);

#ifdef _DEBUG
private:
	BasicEffect*								m_pBasicEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = nullptr;
	ID3D11InputLayout*							m_pInputLayout = nullptr;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);
	_vector Compute_Min();
	_vector Compute_Max();
	OBBDESC Compute_OBB();

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END