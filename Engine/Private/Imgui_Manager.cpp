
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\Public\Imgui_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "Object_Manager.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Cell.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IMPLEMENT_SINGLETON(CImgui_Manager)
CImgui_Manager::CImgui_Manager()
{
}

HRESULT CImgui_Manager::InitImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	ImGui_ImplDX11_Init(*ppDeviceOut, *ppDeviceContextOut);

	m_pDevice = *ppDeviceOut;
	m_DeviceContext = *ppDeviceContextOut;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_DeviceContext);

	return S_OK;
}

HRESULT CImgui_Manager::StartFrame(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	MapMenu_Contents();
	NaviMenu_Contents();

	return S_OK;
}

HRESULT CImgui_Manager::Set_Contents(void)
{

	static bool show_demo_window = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("MapTool Ver.0.0");                        // Create a window called "Hello, world!" and append into it.

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	// 3. 맵툴
	return S_OK;
}

void CImgui_Manager::MapMenu_Contents()
{
	static bool show_another_window = true;
	//vector<const _tchar*> tcharPrototypelist = CComponent_Manager::GetInstance()->Get_ComponentTag(3);
	//vector<const char*> PrototypeList;
	static int TileMove = 0;
	ImGuiStyle* style = &ImGui::GetStyle();
	//static ImGuiTextFilter filter;

	if (show_another_window)
	{
		ImGui::Begin("Map Object", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SliderInt("TileIndex", &TileMove, 0, m_TileNumber);
		if (ImGui::Button("Go", ImVec2(50, 25))) {
			Go_WantTile(TileMove);
		}

		if (ImGui::TreeNode("Tile")) {

			if (ImGui::ListBox(".", &m_TileCnt, TileList, IM_ARRAYSIZE(TileList), 2)) {
				SetUp_ListBoxCount(&m_TileCnt);
				m_Navimode = false;
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Land")) {

			ImGui::BeginListBox(".");

			int SelCnt = 0;
			for (auto& iter : m_LandTagList) {
				if (ImGui::Selectable(iter.Name, &iter.is_Selected)) {
					m_LandCnt = SelCnt;
					iter.is_Selected = true;
					SetUp_ListBoxCount(&m_LandCnt);
					m_Navimode = false;
					break;
				}
				SelCnt++;
			}
			
			int i = 0;
			for (auto& iter : m_LandTagList) {
				if (i != m_LandCnt) {
					iter.is_Selected = false;
				}
				i++;
			}

			ImGui::EndListBox();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Object")) {
			ImGui::BeginListBox(".");

			int SelCnt = 0;
			for (auto& iter : m_ObjTagList) {
				if (ImGui::Selectable(iter.Name, &iter.is_Selected, 0)) {
					m_ObjectCnt = SelCnt;
					iter.is_Selected = true;
					SetUp_ListBoxCount(&m_ObjectCnt);
					m_Navimode = false;
					break;
				}
				SelCnt++;
			}
			
			int i = 0;
			for (auto& iter : m_ObjTagList) {
				if (i != m_ObjectCnt) {
					iter.is_Selected = false;
				}
				i++;
			}

			ImGui::EndListBox();
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Interact")) {
			ImGui::BeginListBox(".");

			int SelCnt = 0;
			for (auto& iter : m_InteractTag) {
				if (ImGui::Selectable(iter.Name, &iter.is_Selected, 0)) {
					m_InteractCnt = SelCnt;
					iter.is_Selected = true;
					SetUp_ListBoxCount(&m_InteractCnt);
					m_Navimode = false;
					break;
				}
				SelCnt++;
			}

			int i = 0;
			for (auto& iter : m_InteractTag) {
				if (i != m_InteractCnt) {
					iter.is_Selected = false;
				}
				i++;
			}

			ImGui::EndListBox();
			ImGui::TreePop();
		}


		if (m_PickObj != nullptr) {
			_float4 PickPos = _float4();
			XMStoreFloat4(&PickPos, m_PickObj->Get_Transform()->Get_State(CTransform::STATE_POSITION));
			static float Pos[3] = { 0.f, 0.f, 0.f };
			ImGui::SliderFloat3("Pos", Pos, 0, 500);


			static _float Rotation = 0.f;
			ImGui::SliderFloat("Rotation", &Rotation, -359, 359);

			if (ImGui::Button("Set", ImVec2(50, 25))) {
				PickPos = _float4(Pos[0], Pos[1], Pos[2], 1.f);
				m_PickObj->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(Rotation));
				m_PickObj->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&PickPos));
			}
		}


		if (ImGui::TreeNode("Portal")) {
			//ImGui::PushItemWidth();

			ImGui::Text("Portal Count : %d", m_PortalCnt);
			ImGui::SameLine(0, 20.f);
			if (ImGui::Button("Push", ImVec2(50, 20))) {
				Push_SavePortal();
			}

			if (ImGui::ListBox(".", &m_SelPortal, PortalList, IM_ARRAYSIZE(PortalList), 5)) {
				m_Navimode = false;
				SetUp_ListBoxCount(&m_SelPortal);
			}
			ImGui::TreePop();
		}

		ImGui::SliderInt("Set Stage", &m_StageIndex, 0, 5);
		if (ImGui::Button("Save", ImVec2(50, 20)))
			SaveData();

		ImGui::SameLine(0.f, 10.f);

		if (ImGui::Button("Load", ImVec2(50, 20))) {
			LoadData();
			m_ObjectCnt = 0;
		}


		if (m_Navimode == false) {
			Create_Object();
			Remote_PickObj();
		}

		ImGui::End();
	}
}

void CImgui_Manager::Create_Object()
{
	m_NextPick = false;
	if (m_ObjectCnt < 0 && m_TileCnt < 0 && m_SelPortal < 0 && m_LandCnt < 0 && m_InteractCnt < 0)
		return;

	int index = max( max( max(m_InteractCnt, m_ObjectCnt), m_SelPortal), max(m_LandCnt, m_TileCnt));

	if (GetAsyncKeyState(VK_LBUTTON) & 0x0001) {
		m_NextPick = ObjectPicking();
		if (m_NextPick == false) {
			TilePicking(index);
		}
	}
}

_bool CImgui_Manager::ObjectPicking()
{
	_bool flag = false;

	for (auto& iter : m_ObjectList) {
		CCollider* CollideSphere = (CCollider*)iter->Object->Get_Component(L"Com_Sphere");
		if (CollideSphere == nullptr)
			continue;

		if (XMVectorGetX(XMVector3Length(CollideSphere->Intersect_Ray()))) {
			CollideSphere->Set_isCollison(true);
			CollideSphere->Set_Color(_float3(1.f, 0.f, 0.f));
			m_PickObj = iter->Object;

			flag = true;
			SetUp_ListBoxCount(&m_ObjectCnt);
			m_ObjectCnt = iter->TagIndex;
		}
		else {
			CollideSphere->Set_isCollison(false);
			CollideSphere->Set_Color(_float3(0.f, 1.f, 0.f));
		}
	}

	for (auto& iter : m_LandList) {
		CCollider* CollideSphere = (CCollider*)iter->Object->Get_Component(L"Com_Sphere");
		if (CollideSphere == nullptr)
			continue;

		if (XMVectorGetX(XMVector3Length(CollideSphere->Intersect_Ray()))) {
			CollideSphere->Set_isCollison(true);
			CollideSphere->Set_Color(_float3(1.f, 0.f, 0.f));
			m_PickObj = iter->Object;

			flag = true;
			SetUp_ListBoxCount(&m_LandCnt);
			m_LandCnt = iter->TagIndex;
		}
		else {
			CollideSphere->Set_isCollison(false);
			CollideSphere->Set_Color(_float3(0.f, 1.f, 0.f));
		}
	}

	if (m_TileCnt >= 0) {
		for (auto& iter : m_TileList) {
			CCollider* CollideSphere = (CCollider*)iter->Object->Get_Component(L"Com_Sphere");
			if (CollideSphere == nullptr)
				continue;
		
			if (XMVectorGetX(XMVector3Length(CollideSphere->Intersect_Ray()))) {
				CollideSphere->Set_isCollison(true);
				CollideSphere->Set_Color(_float3(1.f, 0.f, 0.f));
				m_PickObj = iter->Object;
		
				flag = true;
				SetUp_ListBoxCount(&m_TileCnt);
				m_TileCnt = iter->TagIndex;
			}
			else {
				CollideSphere->Set_isCollison(false);
				CollideSphere->Set_Color(_float3(0.f, 1.f, 0.f));
			}
		}
	}

	for (auto& iter : m_PortalList) {
		CCollider* CollideSphere = (CCollider*)iter->Object->Get_Component(L"Com_Sphere");
		if (CollideSphere == nullptr)
			continue;

		if (XMVectorGetX(XMVector3Length(CollideSphere->Intersect_Ray()))) {
			CollideSphere->Set_isCollison(true);
			CollideSphere->Set_Color(_float3(1.f, 0.f, 0.f));
			m_PickObj = iter->Object;

			flag = true;
			SetUp_ListBoxCount(&m_SelPortal);
			m_SelPortal = iter->TagIndex;
		}
		else {
			CollideSphere->Set_isCollison(false);
			CollideSphere->Set_Color(_float3(0.f, 1.f, 0.f));
		}
	}

	for (auto& iter : m_Interact) {
		CCollider* CollideSphere = (CCollider*)iter->Object->Get_Component(L"Com_Sphere");
		if (CollideSphere == nullptr)
			continue;

		if (XMVectorGetX(XMVector3Length(CollideSphere->Intersect_Ray()))) {
			CollideSphere->Set_isCollison(true);
			CollideSphere->Set_Color(_float3(1.f, 0.f, 0.f));
			m_PickObj = iter->Object;

			flag = true;
			SetUp_ListBoxCount(&m_InteractCnt);
			m_InteractCnt = iter->TagIndex;
		}
		else {
			CollideSphere->Set_isCollison(false);
			CollideSphere->Set_Color(_float3(0.f, 1.f, 0.f));
		}
	}

	return flag;
}

_bool CImgui_Manager::TilePicking(_int index)
{
	_bool flag = false;

	CCollider* Collide = (CCollider*)CObject_Manager::GetInstance()->Get_Component(3, L"Layer_BackGround", L"Com_AABB");
	if (Collide == nullptr)
		return false;

	_vector PickedPos = Collide->Intersect_Ray();

	if (XMVectorGetX(XMVector3Length(PickedPos)) > 0) {
		ImVec2 temp = ImGui::GetMousePos();
		if (temp.x < 300.f)
			return false;

		flag = true;
		//char strtemp[MAX_PATH] = "";
		//strcpy_s(strtemp, PrototypeList[0]);
		//_tchar		szFullPath[MAX_PATH] = TEXT("");
		//MultiByteToWideChar(CP_ACP, 0, PrototypeList[index], (int)strlen(PrototypeList[index]), szFullPath, MAX_PATH);

		CGameObject* Obj = nullptr;
		if (m_ObjectCnt >= 0 && m_TileList.size() > 0)
			Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Object", L"Prototype_GameObject_AllObject", &index);
		else if (m_TileCnt >= 0)
			Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"LandScape", L"Prototype_GameObject_BaseTile", &index);
		else if (m_SelPortal >= 0)
			Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Portal", L"Prototype_GameObject_Portal", &index);
		else if (m_LandCnt >= 0 && m_TileList.size() > 0)
			Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Land", L"Prototype_GameObject_Land", &index);
		else if(m_InteractCnt >= 0 && m_TileList.size() > 0)
			Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Interact", L"Prototype_GameObject_Interact", &index);

		if (Obj != nullptr) {
			CTransform* ObjTrans = Obj->Get_Transform();
			PickedPos = XMVectorSetW(PickedPos, 1.f);
			ObjTrans->Set_State(CTransform::STATE_POSITION, PickedPos);
			DATADESC* Datadesc = new DATADESC;
			Datadesc->Object = Obj;
			Datadesc->TagIndex = index;
			Datadesc->Number = 0;
			if (m_ObjectCnt >= 0 || m_LandCnt >= 0 || m_InteractCnt >= 0) {
				_float ResultDist = (_float)INT_MAX;
				int ResultTileIndex = 0;

				for (auto& iter : m_TileList) {
					_vector TilePos = iter->Object->Get_Transform()->Get_State(CTransform::STATE_POSITION);
					_float Dist = XMVectorGetX(XMVector3Length(Obj->Get_Transform()->Get_State(CTransform::STATE_POSITION) - TilePos));
					if (ResultDist > Dist) {
						ResultDist = Dist;
						ResultTileIndex = iter->Number;
					}
				}
				Datadesc->Number = ResultTileIndex;
				if (m_ObjectCnt >= 0)
					m_ObjectList.push_back(Datadesc);
				else if (m_LandCnt >= 0)
					m_LandList.push_back(Datadesc);
				else if (m_InteractCnt >= 0)
					m_Interact.push_back(Datadesc);
			}
			else if (m_TileCnt >= 0) {
				Datadesc->Number = m_TileNumber;
				m_TileList.push_back(Datadesc);
				m_TileNumber++;
			}
			else if (m_SelPortal >= 0) {
				SetUp_PortalData(Datadesc);
			}
		}
	}
	return flag;
}

void CImgui_Manager::Remote_PickObj()
{
 	if (m_PickObj == nullptr)
		return;

	CTransform* PickTrans = m_PickObj->Get_Transform();

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_3))
		PickTrans->Go_Y(0.005f);
	
	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_4))
		PickTrans->Go_Y(-0.005f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_W))
		PickTrans->Go_Screen_Up(0.01f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_S))
		PickTrans->Go_Screen_Down(0.01f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_A))
		PickTrans->Go_Screen_Left(0.01f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_D))
		PickTrans->Go_Screen_Right(0.01f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_Q))
		PickTrans->Turn(XMVectorSet(0.f, -1.f, 0.f, 0.f), 0.01f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_E))
		PickTrans->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.01f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_Z))
		PickTrans->Scailing(0.005f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_X))
		PickTrans->Scailing(-0.005f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_DELETE)) {
		CGameObject* Temp = m_PickObj;
		m_PickObj = nullptr;

		int i = 0;
		if (m_ObjectCnt >= 0) {
			for (auto&iter : m_ObjectList) {
				if (iter->Object == Temp) {
					m_ObjectList.erase(m_ObjectList.begin() + i);
					break;
				}
				else
					i++;
			}
		}
		else if (m_LandCnt >= 0) {
			for (auto&iter : m_LandList) {
				if (iter->Object == Temp) {
					m_LandList.erase(m_LandList.begin() + i);
					break;
				}
				else
					i++;
			}
		}
		else if (m_InteractCnt >= 0) {
			for (auto&iter : m_Interact) {
				if (iter->Object == Temp) {
					m_Interact.erase(m_Interact.begin() + i);
					break;
				}
				else
					i++;
			}
		}
		else if (m_TileCnt >= 0) {
			for (auto&iter : m_TileList) {
				if (iter->Object == Temp) {
					m_TileList.erase(m_TileList.begin() + i);
					break;
				}
				else
					i++;
			}
		}
		else if (m_SelPortal >= 0) {
			for (auto&iter : m_PortalList) {
				if (iter->Object == Temp) {
					m_PortalList.erase(m_PortalList.begin() + i);
					break;
				}
				else
					i++;
			}
			if (m_PortalCnt == 1) {
				m_LinkDesc.Me = nullptr;
				m_PortalCnt--;
			}
			if (m_PortalCnt == 2) {
				m_LinkDesc.You = nullptr;
				m_PortalCnt--;
			}
		}
		
		Temp->Set_Dead();
	}
}

void CImgui_Manager::NaviMenu_Contents()
{
	static bool show_navi_window = true;

	static int selectCnt = -1;


	ImGuiStyle* style = &ImGui::GetStyle();
	static ImGuiTextFilter filter;

	if (show_navi_window)
	{
		ImGui::Begin("Navi", &show_navi_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Checkbox("isNaviMode", &m_Navimode);

		ImGui::Combo(" ", &m_CellType, Cell_TypeIndex, IM_ARRAYSIZE(Cell_TypeIndex));

		if (m_Navimode == true) {
			if (GetAsyncKeyState(VK_LBUTTON) & 0x0001) {

				Navi_Create();
			}

			if (GetAsyncKeyState(VK_END) & 0x0001)
				Push_Cell();

			if (GetAsyncKeyState(VK_DELETE) & 0x0001)
				Undo_NaviBox();

			Remote_Navi();
		}

		if (ImGui::Button("Save", ImVec2(50, 20)))
			SaveNavi();

		ImGui::SameLine(0.f, 10.f);

		if (ImGui::Button("Load", ImVec2(50, 20))) {
			LoadNavi();
		}

		ImGui::Text("NumPad 1, 2 : Height");
		ImGui::Text("NumPad 4, 5, 6, 8 : Moves Created Last NaviMesh");
		ImGui::Text("END : Create Cell");
		ImGui::Text("DELETE : Delete Last Cell");

		ImGui::End();
	}
}


_bool CImgui_Manager::Remote_Navi()
{
	if (m_PickNavi == nullptr || m_NaviCount == 0)
		return false;

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD8))
		m_PickNavi->Get_Transform()->Go_Screen_Up(0.003f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD5))
		m_PickNavi->Get_Transform()->Go_Screen_Down(0.003f);

	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD4))
		m_PickNavi->Get_Transform()->Go_Screen_Left(0.003f);
	
	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD6))
		m_PickNavi->Get_Transform()->Go_Screen_Right(0.003f);

	return true;
}

_bool  CImgui_Manager::Navi_Create()
{
	_bool flag = false;
	_vector PickedPos = _vector();
	
	list<CGameObject*>* NaviList = CObject_Manager::GetInstance()->Get_GameObjectList(3, L"NaviFlag");

	if (NaviList != nullptr) {
		for (auto& iter : *NaviList) {
			CCollider* ObjCol = (CCollider*)iter->Get_Component(L"Com_Sphere");
			_vector PickObj = ObjCol->Intersect_Ray();
			//ObjCol->Set_isCollison(false);
			if (XMVectorGetX(XMVector3Length(PickObj)) > 0) {
				ObjCol->Set_isCollison(true);
				PickedPos = iter->Get_Transform()->Get_State(CTransform::STATE_POSITION);
				flag = true;
				break;
			}
		}
	}

	CCollider* Collide = (CCollider*)CObject_Manager::GetInstance()->Get_Component(3, L"Layer_BackGround", L"Com_AABB");
	if (Collide == nullptr)
		return false;

	if(flag == false)
		PickedPos = Collide->Intersect_Ray();

	if (XMVectorGetX(XMVector3Length(PickedPos)) > 0 && m_NaviCount < 3) {
		CGameObject* Obj = nullptr;
		if ((Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"NaviFlag", L"Prototype_GameObject_NaviFlag", &PickedPos)) != nullptr) {
			CCollider* ObjCollide = (CCollider*)Obj->Get_Component(L"Com_Sphere");

			CTransform* ObjTrans = Obj->Get_Transform();
			PickedPos = XMVectorSetW(PickedPos, 1.f);
			ObjTrans->Set_State(CTransform::STATE_POSITION, PickedPos);
			m_PickNavi = Obj;
			m_CreateNavi[m_NaviCount] = Obj;
			ObjCollide->Set_Color(_float3(rand() % 11 / 10.f, rand() % 11 / 10.f, rand() % 11 / 10.f));
			m_NaviCount++;
		}
	}
	return true;
}

_bool CImgui_Manager::Push_Cell()
{
	if (m_NaviCount < 3)
		return false;

	CTransform* CPos;
	_float3 fPos;

	CPos = m_CreateNavi[0]->Get_Transform();
	XMStoreFloat3(&fPos, CPos->Get_State(CTransform::STATE_POSITION));
	m_Point.PointA = fPos;

	CPos = m_CreateNavi[1]->Get_Transform();
	XMStoreFloat3(&fPos, CPos->Get_State(CTransform::STATE_POSITION));
	m_Point.PointB = fPos;

	CPos = m_CreateNavi[2]->Get_Transform();
	XMStoreFloat3(&fPos, CPos->Get_State(CTransform::STATE_POSITION));
	m_Point.PointC = fPos;
	m_Point.Index = m_CellType;

	m_Cell.push_back(m_Point);
	CNavigation::GetInstance()->Push_Cell(m_pDevice, m_DeviceContext, &m_Point);

	m_NaviCount = 0;
	m_PickNavi = nullptr;

	return true;
}

void CImgui_Manager::Undo_NaviBox()
{
	if (m_NaviCount != 0)
		return;

	CNavigation::GetInstance()->Remove_LastCell();
	m_Cell.pop_back();
}

_bool CImgui_Manager::SaveNavi()
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("../DataNavi/DataNavi.dat"), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);

	int i = 0;
	_tchar SzPath[MAX_PATH] = L"";
	while (hFile == INVALID_HANDLE_VALUE) {
		wsprintf(SzPath, TEXT("../DataNavi/DataNavi%d.dat"), i);
		hFile = CreateFile(SzPath, GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		i++;
	}

	if (0 == hFile) {
		MSGBOX("FAILDE OPEN FILE !!!");
		return false;
	}

	for (auto iter : m_Cell) {
		WriteFile(hFile, &iter, sizeof(CELLDESC), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MSGBOX("SUCCESS !!!");
	return true;
}

_bool CImgui_Manager::LoadNavi()
{
	CTransform* BaseTile = CObject_Manager::GetInstance()->Get_Transform(3, L"Layer_BackGround");
	if (BaseTile == nullptr) {
		MSGBOX("FAILED BaseTile Transform")
		return false;
	}

	CNavigation::GetInstance()->Initialize(m_pDevice, m_DeviceContext, TEXT("../DataNavi/DataNavi.dat"), BaseTile);

	vector<CCell*> NaviCell = CNavigation::GetInstance()->Get_Cells();
	CELLDESC celldesc;

	for (auto& iter : NaviCell) {
		for (int j = 0; j < 3; j++) {
			CGameObject* NaviMesh = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"NaviFlag", L"Prototype_GameObject_NaviFlag");
			if (NaviMesh == nullptr) {
				MSGBOX("LOAD FAILED !!!!");
				return false;
			}
			NaviMesh->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(iter->Get_fPoint(j).x, iter->Get_fPoint(j).y, iter->Get_fPoint(j).z, 1.f));
		}

		celldesc.PointA = iter->Get_fPoint(0);
		celldesc.PointB = iter->Get_fPoint(1);
		celldesc.PointC = iter->Get_fPoint(2);
		celldesc.Index = iter->Get_TypeIndex();
		m_Cell.push_back(celldesc);
	}

	MSGBOX("GOOD !");
	return true;
}

_bool CImgui_Manager::CellPicking()
{
	vector<CCell*> Cells = CNavigation::GetInstance()->Get_Cells();

	for (auto& iter : Cells) {

	}


	return true;
}

void CImgui_Manager::SetUp_PortalData(DATADESC* Portal)
{
	_vector PortalPos = Portal->Object->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_float ResultDist = (_float)INT_MAX;
	int ResultTileIndex = 0;

	for (auto& iter : m_TileList) {
		_vector TilePos = iter->Object->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		_float Dist = XMVectorGetX(XMVector3Length(PortalPos - TilePos));
		if (ResultDist > Dist) {
			ResultDist = Dist;
			ResultTileIndex = iter->Number;
		}
	}

	Portal->Number = ResultTileIndex;
	Portal->Object->Set_Alpha(0.5f);
	m_PortalList.push_back(Portal);

	if (m_PortalCnt == 0)
		m_LinkDesc.Me = Portal;
	else
		m_LinkDesc.You = Portal;
	m_PortalCnt++;
}

_bool CImgui_Manager::Push_SavePortal()
{
	if (m_PortalCnt == 2) {
		SAVEPORTAL SavePortal;
		SavePortal.TagIndex = m_LinkDesc.Me->TagIndex;
		SavePortal.TileIndex = m_LinkDesc.Me->Number;
		SavePortal.LinkTileIndex = m_LinkDesc.You->Number;
		XMStoreFloat3(&SavePortal.LinkPos, m_LinkDesc.You->Object->Get_Transform()->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat3(&SavePortal.LinkOutDir, m_LinkDesc.You->Object->Get_Transform()->Get_State(CTransform::STATE_LOOK));
		SavePortal.WorldMtx = m_LinkDesc.Me->Object->Get_Transform()->Get_WorldFloat4x4();
		m_SavePortalList.push_back(SavePortal);

		SAVEPORTAL SavePortal2;
		SavePortal2.TagIndex = m_LinkDesc.You->TagIndex;
		SavePortal2.TileIndex = m_LinkDesc.You->Number;
		SavePortal2.LinkTileIndex = m_LinkDesc.Me->Number;
		XMStoreFloat3(&SavePortal2.LinkPos, m_LinkDesc.Me->Object->Get_Transform()->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat3(&SavePortal2.LinkOutDir, m_LinkDesc.Me->Object->Get_Transform()->Get_State(CTransform::STATE_LOOK));
		SavePortal2.WorldMtx = m_LinkDesc.You->Object->Get_Transform()->Get_WorldFloat4x4();
		m_SavePortalList.push_back(SavePortal2);

		m_PortalCnt = 0;
		
		m_LinkDesc.Me->Object->Set_Alpha(1.f);
		m_LinkDesc.You->Object->Set_Alpha(1.f);
		
		return true;
	}

	return false;
}

void CImgui_Manager::SetUp_ListBoxCount(_int * flag)
{
	if (flag == &m_ObjectCnt) {
		m_TileCnt = -1;
		m_SelPortal = -1;
		m_LandCnt = -1;
		m_InteractCnt = -1;
	}

	if (flag == &m_TileCnt) {
		m_SelPortal = -1;
		m_LandCnt = -1;
		m_ObjectCnt = -1;
		m_InteractCnt = -1;
	}

	if (flag == &m_SelPortal) {
		m_TileCnt = -1;
		m_LandCnt = -1;
		m_ObjectCnt = -1;
		m_InteractCnt = -1;
	}

	if (flag == &m_LandCnt) {
		m_SelPortal = -1;
		m_TileCnt = -1;
		m_ObjectCnt = -1;
		m_InteractCnt = -1;
	}

	if (flag == &m_InteractCnt) {
		m_SelPortal = -1;
		m_TileCnt = -1;
		m_ObjectCnt = -1;
		m_LandCnt = -1;
	}
}

_bool CImgui_Manager::SaveData()
{
	_ulong		dwByte = 0;
	//CREATE_ALWAYS
	HANDLE		hFile = CreateFile(TEXT("../Data/MapData.dat"), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);

	int i = 0;
	_tchar SzPath[MAX_PATH] = L"";
	while(hFile == INVALID_HANDLE_VALUE) {
		wsprintf(SzPath, TEXT("../Data/MapData%d.dat"), i);
		hFile = CreateFile(SzPath, GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		i++;
	}

	if (0 == hFile) {
		MSGBOX("FAILDE OPEN FILE !!!");
		return false;
	}

	//클래스 태그, 월드매트릭스
	LOADDATA LoadData;
	LoadData.TileCnt = m_TileList.size();
	LoadData.ObjCnt = m_ObjectList.size();
	LoadData.LandCnt = m_LandList.size();
	LoadData.PortalCnt = m_SavePortalList.size();
	LoadData.InteractCnt = m_Interact.size();
	WriteFile(hFile, &(LoadData), sizeof(LOADDATA), &dwByte, nullptr);

	for (auto iter : m_TileList) {
		SAVETILE SaveDesc;
		SaveDesc.TagIndex = iter->TagIndex;
		SaveDesc.Number = iter->Number;
		SaveDesc.WorldMtx = iter->Object->Get_Transform()->Get_WorldFloat4x4();
		WriteFile(hFile, &(SaveDesc), sizeof(SAVETILE), &dwByte, nullptr);
	}

	for (auto iter : m_SavePortalList)
		WriteFile(hFile, &(iter), sizeof(SAVEPORTAL), &dwByte, nullptr);


	for (auto iter : m_ObjectList) {
		SAVEDESC SaveDesc;
		SaveDesc.StageIndex = m_StageIndex;
		SaveDesc.TagIndex = iter->TagIndex;
		SaveDesc.Number = iter->Number;
		SaveDesc.WorldMtx = iter->Object->Get_Transform()->Get_WorldFloat4x4();

		WriteFile(hFile, &(SaveDesc), sizeof(SAVEDESC), &dwByte, nullptr);
	}

	for (auto iter : m_LandList) {
		SAVEDESC SaveDesc;
		SaveDesc.StageIndex = m_StageIndex;
		SaveDesc.TagIndex = iter->TagIndex;
		SaveDesc.Number = iter->Number;
		SaveDesc.WorldMtx = iter->Object->Get_Transform()->Get_WorldFloat4x4();

		WriteFile(hFile, &(SaveDesc), sizeof(SAVEDESC), &dwByte, nullptr);
	}

	for (auto& iter : m_Interact) {
		SAVEDESC SaveDesc;
		SaveDesc.StageIndex = m_StageIndex;
		SaveDesc.TagIndex = iter->TagIndex;
		SaveDesc.Number = iter->Number;
		SaveDesc.WorldMtx = iter->Object->Get_Transform()->Get_WorldFloat4x4();

		WriteFile(hFile, &(SaveDesc), sizeof(SAVEDESC), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MSGBOX("SUCCESS !!");

	return true;
}

_bool CImgui_Manager::LoadData()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(TEXT("../Data/MapData.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile) {
		MSGBOX("FAILED !!!");
		return false;
	}

	if (INVALID_HANDLE_VALUE == hFile) {
		MSGBOX("FAILED !!!");
		return false;
	}

	LOADDATA LoadData;

	ReadFile(hFile, &LoadData, sizeof(LOADDATA), &dwByte, nullptr);
	if (dwByte == 0)
		return false;

	for (int i = 0; i < LoadData.TileCnt; i++) {
		SAVETILE SaveTile;
		ReadFile(hFile, &SaveTile, sizeof(SAVETILE), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		
		CGameObject* Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"LandScape", L"Prototype_GameObject_BaseTile", &SaveTile.TagIndex);

		DATADESC* Data = new DATADESC;
		Data->TagIndex = SaveTile.TagIndex;
		Data->Number = SaveTile.Number;
		Data->Object = Obj;
		m_TileNumber = Data->Number;
		Obj->Get_Transform()->Set_WorldMTX(SaveTile.WorldMtx);

		m_TileList.push_back(Data);
	}

	for (int i = 0; i < LoadData.PortalCnt; i++) {
		SAVEPORTAL SavePortal;
		ReadFile(hFile, &SavePortal, sizeof(SAVEPORTAL), &dwByte, nullptr);
		if (0 == dwByte)
			break;
	
		m_SavePortalList.push_back(SavePortal);
			
		CGameObject* Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Portal", L"Prototype_GameObject_Portal", &SavePortal.TagIndex);
		DATADESC* Data = new DATADESC;
		Data->TagIndex = SavePortal.TagIndex;
		Data->Number = SavePortal.TileIndex;
		Data->Object = Obj;
		Obj->Get_Transform()->Set_WorldMTX(SavePortal.WorldMtx);
		
		m_PortalList.push_back(Data);
	}

	for (int i = 0; i < LoadData.ObjCnt; i++)
	{
		SAVEDESC SaveData;

		ReadFile(hFile, &SaveData, sizeof(SAVEDESC), &dwByte, nullptr);
		if (0 == dwByte)
			break;
		CGameObject* Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Object", L"Prototype_GameObject_AllObject", &SaveData.TagIndex);

		DATADESC* Data = new DATADESC;
		Data->TagIndex = SaveData.TagIndex;
		Data->Number = SaveData.Number;
		Data->Object = Obj;

		Obj->Get_Transform()->Set_WorldMTX(SaveData.WorldMtx);

		m_ObjectList.push_back(Data);
	}

	for (int i = 0; i < LoadData.LandCnt; i++)
	{
		SAVEDESC SaveData;

		ReadFile(hFile, &SaveData, sizeof(SAVEDESC), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CGameObject* Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Land", L"Prototype_GameObject_Land", &SaveData.TagIndex);

		DATADESC* Data = new DATADESC;
		Data->TagIndex = SaveData.TagIndex;
		Data->Number = SaveData.Number;
		Data->Object = Obj;

		Obj->Get_Transform()->Set_WorldMTX(SaveData.WorldMtx);

		m_LandList.push_back(Data);
	}

	for (int i = 0; i < LoadData.InteractCnt; i++)
	{
		SAVEDESC SaveData;

		ReadFile(hFile, &SaveData, sizeof(SAVEDESC), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CGameObject* Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Interact", L"Prototype_GameObject_Interact", &SaveData.TagIndex);

		DATADESC* Data = new DATADESC;
		Data->TagIndex = SaveData.TagIndex;
		Data->Number = SaveData.Number;
		Data->Object = Obj;

		Obj->Get_Transform()->Set_WorldMTX(SaveData.WorldMtx);

		m_Interact.push_back(Data);
	}

	CloseHandle(hFile);

	MSGBOX("SUCCESS !!");

	return true;
}

void CImgui_Manager::Go_WantTile(int flag)
{
	for (auto& iter : m_TileList) {
		if (iter->Number == flag) {
			_vector TilePos = iter->Object->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			_vector temp = XMVectorSetY(TilePos, 20.f);
			CTransform* trans = CObject_Manager::GetInstance()->Get_Transform(3, L"Layer_Camera");
			if (trans != nullptr) {
				trans->Set_State(CTransform::STATE_POSITION, temp);
			}
		}
	}
}

HRESULT CImgui_Manager::Render()
{
	// Imgui render옵션이 켜져 있다면
	if (m_bImguiEnable) {
		StartFrame();
		Set_Contents();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return S_OK;
}

void CImgui_Manager::InitWin32(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
}

LRESULT CImgui_Manager::WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}

void CImgui_Manager::Shutdown(void)
{

}

void CImgui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

