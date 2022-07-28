
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "..\Public\Imgui_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "Object_Manager.h"
#include "GameObject.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IMPLEMENT_SINGLETON(CImgui_Manager)
CImgui_Manager::CImgui_Manager()
{
}

HRESULT CImgui_Manager::InitImGui(ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	ImGui_ImplDX11_Init(*ppDeviceOut, *ppDeviceContextOut);
	return S_OK;
}

HRESULT CImgui_Manager::StartFrame(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	MapMenu_Contents();
	return S_OK;
}

HRESULT CImgui_Manager::Set_Contents(void)
{
	static bool show_demo_window = true;


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("MapTool Ver0.0");                        // Create a window called "Hello, world!" and append into it.

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
	static int selectCnt = -1;

	ImGuiStyle* style = &ImGui::GetStyle();
	static ImGuiTextFilter filter;

	if (show_another_window)
	{
		ImGui::Begin("Map Object", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		filter.Draw(" ", 200);

		if (ImGui::TreeNode("Object")) {
			//ImGui::PushItemWidth();
			
			if (ImGui::ListBox("", &selectCnt, PrototypeList, IM_ARRAYSIZE(PrototypeList), 5)) {
			}
			ImGui::TreePop();

			if (ImGui::Button("Save", ImVec2(50, 20)))
				SaveData();

			ImGui::SameLine(0.f, 10.f);

			if (ImGui::Button("Load", ImVec2(50, 20))) {
				LoadData();
				selectCnt = 0;
			}
		}
		Create_Object(selectCnt);
		Remote_PickObj();
		
		ImGui::End();
	}
}

void CImgui_Manager::Create_Object(_int index)
{
	m_NextPick = false;
	if (index < 0)
		return;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x0001) {
		m_NextPick = ObjectPicking();
		if(m_NextPick == false)
			TilePicking(index);
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
			m_PickObj = iter->Object;

			flag = true;
		}
		else
			CollideSphere->Set_isCollison(false);
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

	if( XMVectorGetX(XMVector3Length(PickedPos)) > 0) {
		flag = true;
		//char strtemp[MAX_PATH] = "";
		//strcpy_s(strtemp, PrototypeList[0]);
		_tchar		szFullPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, PrototypeList[index], (int)strlen(PrototypeList[index]), szFullPath, MAX_PATH);
		CGameObject* Obj = nullptr;
		if ((Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"ObjectList", szFullPath, &PickedPos)) != nullptr) {
			CTransform* ObjTrans = Obj->Get_Transform();
			PickedPos = XMVectorSetW(PickedPos, 1.f);
			ObjTrans->Set_State(CTransform::STATE_POSITION, PickedPos);
			DATADESC* Datadesc = new DATADESC;
			Datadesc->Object = Obj;
			Datadesc->TagIndex = index;
			m_ObjectList.push_back(Datadesc);
		}
	}
	return flag;
}

void CImgui_Manager::Remote_PickObj()
{
	if (m_PickObj == nullptr)
		return;
	CTransform* PickTrans = m_PickObj->Get_Transform();

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
		for (auto&iter : m_ObjectList) {
			if (iter->Object == Temp){
				m_ObjectList.erase(m_ObjectList.begin() + i);
				break;
			}
			else {
				i++;
			}
		}

		Temp->Set_Dead();
	}
}

_bool CImgui_Manager::SaveData()
{
	_ulong		dwByte = 0;

	HANDLE		hFile = CreateFile(TEXT("../Data/MapData.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile) {
		return false;
	}

	//클래스 태그, 월드매트릭스

	for (auto iter : m_ObjectList) {
		SAVEDESC SaveDesc;

		SaveDesc.TagIndex = iter->TagIndex;
		SaveDesc.WorldMtx = iter->Object->Get_Transform()->Get_WorldFloat4x4();

		WriteFile(hFile, &(SaveDesc), sizeof(SAVEDESC), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	return true;
}

_bool CImgui_Manager::LoadData()
{
	_ulong			dwByte = 0;

	HANDLE			hFile = CreateFile(TEXT("../Data/MapData.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return false;

	while (true)
	{
		SAVEDESC SaveData;

		ReadFile(hFile, &SaveData, sizeof(SAVEDESC), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		_tchar		szFullPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, PrototypeList[SaveData.TagIndex], (int)strlen(PrototypeList[SaveData.TagIndex]), szFullPath, MAX_PATH);

		CGameObject* Obj = CObject_Manager::GetInstance()->Add_GameObjToLayer(3, L"Object", szFullPath);

		DATADESC* Data = new DATADESC;
		Data->TagIndex = SaveData.TagIndex;
		Data->Object = Obj;

		Obj->Get_Transform()->Set_WorldMTX(SaveData.WorldMtx);

		m_ObjectList.push_back(Data);
	}

	CloseHandle(hFile);

	return true;
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
