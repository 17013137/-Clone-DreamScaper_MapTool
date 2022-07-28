#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;

static const char* PrototypeList[]{ "Prototype_GameObject_Bed", "Prototype_GameObject_House_00", "Prototype_GameObject_House_01", "Prototype_GameObject_House_02", "Prototype_GameObject_House_03" , "Prototype_GameObject_House_04",
"Prototype_GameObject_ParkingRoad", "Prototype_GameObject_CampgroundRoad" };

class ENGINE_DLL CImgui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)

public:
	typedef struct DATADESC {
		_uint TagIndex;
		CGameObject* Object;
	}DATADESC;

	typedef struct SAVEDESC {
		_uint TagIndex;
		_float4x4 WorldMtx;
	}SAVEDESC;

public:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT InitImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT StartFrame(void);
	HRESULT Set_Contents(void);
	void MapMenu_Contents();
	void Create_Object(_int index);
	_bool ObjectPicking();
	_bool TilePicking(_int index);
	void Remote_PickObj();

	_bool SaveData();
	_bool LoadData();

public:
	HRESULT Render(void);
	void InitWin32(HWND hWnd);
	LRESULT WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Shutdown(void);

private:
	bool m_bImguiEnable = false;
	_bool m_NextPick = false;
	CGameObject* m_PickObj = nullptr;
	vector<DATADESC*> m_ObjectList;

public:
	void OnOffImgui(void) { m_bImguiEnable = !m_bImguiEnable; }
	bool isImguiEnable() { return m_bImguiEnable; }


public:
	virtual void Free() override;
};

END