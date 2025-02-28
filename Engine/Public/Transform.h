#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
	enum JUMP_STATE { JUMP_START, JUMP_UP, JUMP_DOWN, JUMP_ARRIVE, ANUM_SKILL, JUMP_END};
	enum ANIM {ANIM_SKILL, ANIM_REBOUND, ANIM_END};
public:
	typedef struct tagTransformDesc
	{
		_double			SpeedPerSec;
		_double			RotationPerSec;
	}TRANSFORMDESC;

	typedef struct JUMPDESC {
		_float fTime = 0.f;
		_float fHegiht = 0.f;
		_float fPower = 5.f;
		_float fSpeed = 0.f;
		_bool isJump = false;
		_bool isDoubleJump = false;
		_vector vOriginPos;
		_vector vPrevPos;
		_float doubleJumpY = 0.f;
		_float MaxY = 0.f;
	}JUMPDESC;

	typedef struct SITDESC {
		_float fTime = 0.f;
		_float fSize = 0.f;
		_float fPower = 0.f;
		_bool isSit = false;
	}SITDESC;

	typedef struct SLIDEDESC {
		_bool isSlide = false;
		_bool isLeft = false;
		_bool isTurn = false;
		_vector vPrevPos;
		_float fSlideDuration = 0.f;
		_float fTime = 0.f;
		_float fMaxTurn = 5.f;
		_float fNowTurn = 0.f;
		_float fTurnSpeed = 0.25f;
		_float fSlideSpeed = 0.f;
	}SLIDEDESC;

	typedef struct ANIMDESC{
		_bool isStart = false;
		_float fTimeAcc = 0.f;
		_float fDuration = 0.f;
		_float Rotation_Speed = 0.f;
		_float fSpeed = 1.f;
		_bool flag = false;
	}ANIMDESC;
	
	typedef struct ACCELDESC {
		_bool isStart = false;
		_bool isRight = false;
		_double TotalTime = 0.0;
		_double Speed = 0.0;
		_double MaxSpeed = 0.0;
		_double WhenSlow = 0.0;
		
	}ACCELDESC;

	typedef struct RESERVEDESC {
		_bool isStart = false;
		_bool isRight = false;
		_double TotalTime = 0.0;
	}RESERVEDESC;

	typedef struct AIRINFODESC {
		_float TotalX = 0.f;
		_float TotalY = 0.f;

	}AIRINFODESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CTransform() = default;

public: // Getter 
	_vector Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
	}

	_float3 Get_Scale();

	_matrix Get_WorldMatrix();
	_matrix Get_WorldMatrixInverse();
	_float4x4 Get_WorldFloat4x4();
	_float4x4 Get_WorldFloat4x4_TP();

public: // Setter 
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
	}

	void Set_WorldMTX(_float4x4 _WorldMatrix) { m_WorldMatrix = _WorldMatrix; }
	
public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);

public:
	HRESULT Bind_WorldMatrixOnShader(class CShader* pShader, const char* pConstantName);

public:
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_BackWard(_double TimeDelta);

	HRESULT Go_MyStraight(_double TimeDelta);
	HRESULT Go_MyBackWard(_double TimeDelta);
	HRESULT Go_MyLeft(_double TimeDelta);
	HRESULT Go_MyRight(_double TimeDelta);

	HRESULT Go_Jumping(_fvector PrevPos, _float Speed, _double TimeDelta);
	HRESULT Go_Slide(_double TimeDelta);
	HRESULT Go_SitDown(_double TimeDelta);
	HRESULT Go_SitUp(_double TimeDelta);
	HRESULT Go_JumpStartAnim(_double TimeDelta);
	HRESULT Go_JumpEndAnim(_double TimeDelta);

	HRESULT Go_X(_double TimeDelta);
	HRESULT Go_Y(_double TimeDelta);
	HRESULT Go_Z(_double TimeDelta);

	HRESULT Go_Screen_Up(_double Timedelta);
	HRESULT Go_Screen_Down(_double Timedelta);
	HRESULT Go_Screen_Left(_double Timedelta);
	HRESULT Go_Screen_Right(_double Timedelta);


	HRESULT Skill_Anim(_double TimeDelta, _bool *isSkill);
	HRESULT Aim_ReBounding(_double ReboundPer, _bool* isAttack, _double TimeDelta);

	HRESULT Scaled(const _float3& vScale);
	HRESULT Scailing(_double TimeDelta);

	HRESULT AccelTurn(_fvector _vAxis, _double Timedelta, _bool* isRight);
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _double Radian);
	HRESULT MyRotation(_fvector vAxis, _double Radian);
	HRESULT Reserve_Turn(_fvector vAxis, _double Reserve, _double TimeDelta);
	HRESULT TurnAndMove(_fvector vAxis, _fvector vDir, _double TimeDelta);


	HRESULT MyLookSet(_fvector vLook);
	HRESULT MyLook(_vector vTargetPos, _float Speed, _double Timedelta);
	HRESULT MyAirLook(_vector vTargetPos, _float Speed, _double Timedelta);
	HRESULT LookAt(_vector vTargetPos);
	HRESULT Reset_AxisY();

	HRESULT BulletShoot(_fvector _Look, _fvector _Right, _fvector muzzle);

private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;
	JUMPDESC m_JumpDesc;
	SITDESC	m_SitDesc;
	SLIDEDESC m_SlideDesc;
	ANIMDESC m_JumpAnimDesc[JUMP_END];
	ANIMDESC m_AnimDesc[ANIM_END];
	ACCELDESC m_AccelDesc;
	RESERVEDESC m_ResverDesc;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END