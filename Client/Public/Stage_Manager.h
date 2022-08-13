#pragma once

class CStage_Manager
{

	enum CITEM_TEXTURE { CT_BASEBALLBAT, CT_BOXINGGLOVE, CT_END };
public:
	static CStage_Manager*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CStage_Manager;

		return m_pInstance;
	}
	static void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static		CStage_Manager*				m_pInstance;

	/*For Circle Item*/
private:
	list<CGameObject*>			Portal_List{};

public:
	void Add_Portal(CGameObject* pPortal) { Portal_List.push_back(pPortal); }
	_vector Player_FPressing();

public:
	CStage_Manager();
	~CStage_Manager();

public:
	void Free();
};

