#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CPanel : public CBase
{
private:
	explicit CPanel();
	virtual ~CPanel();

public:
	void Display_Editor();

public:
	void		Add_EditorField(const char* pTag, DATATYPE type, void* pData)
	{
		m_EditorFieldList.push_back({ pTag, type, pData });
	}
	void		Add_EditorField(EDITORFIELD field)
	{
		m_EditorFieldList.push_back(field);
	}

public:
	list<EDITORFIELD>* Get_EditorFieldList() { return &m_EditorFieldList; }
	void								 Set_PanelName(const char* pName) { strcpy_s(szPanelName, pName); }

public:
	static CPanel* Create(const char* pName = nullptr);

public:
	bool bDisplayEditor;

private:
	list<EDITORFIELD> m_EditorFieldList;
	char szPanelName[32];

public:
	virtual void Free();
};

END