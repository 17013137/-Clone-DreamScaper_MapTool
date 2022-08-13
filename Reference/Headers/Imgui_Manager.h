#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;
static const char* TileList[]{ "Normal" };

static const char* PortalList[]{ "Normal", "Secret", "Hidden" };

class ENGINE_DLL CImgui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)
public:
	char* Cell_TypeIndex[5]{ "Type1", "Type2", "Type3", "Type4", "Type5" };

public:
	typedef struct Item {
		char Name[MAX_PATH];
		bool is_Selected = false;
	}Item;

	typedef struct LOADDATA {
		int TileCnt;
		int ObjCnt;
		int PortalCnt;
	}LOADDATA;

	typedef struct DATADESC {
		_uint TagIndex;
		_uint Number;
		CGameObject* Object;
	}DATADESC;

	typedef struct SAVEDESC {
		_uint TagIndex;
		_uint Number;
		XMFLOAT4X4 WorldMtx;
	}SAVEDESC;
	
	typedef struct SAVETILE {
		_uint TagIndex;
		_uint Number;
		XMFLOAT4X4 WorldMtx;
	}SAVETILE;

	typedef struct LINKPORTAL {
		DATADESC* Me;
		DATADESC* You;
	}LINKPORTAL;

	typedef struct SAVEPORTAL {
		_uint TagIndex;
		_uint TileIndex;
		_uint LinkTileIndex;
		XMFLOAT3 LinkPos;
		XMFLOAT3 LinkOutDir;
		XMFLOAT4X4 WorldMtx;
	}SAVEPORTAL;

public:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT InitImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT StartFrame(void);
	HRESULT Set_Contents(void);
	void InitWin32(HWND hWnd);
	LRESULT WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT Render(void);
	void Shutdown(void);

public:
	//맵툴함수
	void MapMenu_Contents();
	void Create_Object();
	_bool ObjectPicking();
	_bool TilePicking(_int index);
	void Remote_PickObj();
	_bool SaveData();
	_bool LoadData();
	void Go_WantTile(int flag);

	// _bool SaveTile();
	// _bool LoadTile();

	//네비메쉬함수
	void NaviMenu_Contents();
	_bool Remote_Navi();
	_bool Push_Cell();
	void Undo_NaviBox();
	_bool SaveNavi();
	_bool LoadNavi();


	//포탈관련함수
	void SetUp_PortalData(DATADESC* Portal);
	_bool Push_SavePortal();
private:
	_bool Navi_Create();

public:
	bool m_bImguiEnable = false;
	_bool m_NextPick = false;
	CGameObject* m_PickObj = nullptr;
	vector<DATADESC*> m_ObjectList;
	vector<DATADESC*> m_TileList;
	vector<Item> m_ObjListBox;
	_int m_TileCnt = -1;
	_int m_ObjectCnt = -1;
	_int m_TileNumber = 0;

	//네비
	_bool m_Navimode = false;
	_uint m_NaviCount = 0;
	CGameObject* m_PickNavi = nullptr;
	vector<CELLDESC> m_Cell;
	CGameObject* m_CreateNavi[3] = { nullptr };
	CELLDESC m_Point = { _float3() };
	_int m_CellType = 0;

	//포탈관련
	vector<DATADESC*> m_PortalList;
	vector<SAVEPORTAL> m_SavePortalList;
	_int m_SelPortal = -1;
	_int m_PortalCnt = 0;
	LINKPORTAL m_LinkDesc;

	_bool m_isOpenNode = false;
public:
	void OnOffImgui(void) { m_bImguiEnable = !m_bImguiEnable; }
	bool isImguiEnable() { return m_bImguiEnable; }
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;

public:
	virtual void Free() override;
};

END