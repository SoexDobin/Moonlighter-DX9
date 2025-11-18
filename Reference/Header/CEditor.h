#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPanel;

class ENGINE_DLL CEditor : public CBase
{
	DECLARE_SINGLETON(CEditor)

private:
	explicit CEditor();
	explicit CEditor(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEditor();

public:
	bool Editor_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HRESULT Ready_Editor(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev);
	void Update_Editor();
	void Render_Editor();

	void Display_Editor(EDITORFIELD field);

public:
	CPanel* Create_Panel(const char* pName);
	CPanel* Get_Panel(const char*);
	CPanel* Find_Panel(const char* pName);

	void			Add_EditorField(const char* pName, EDITORFIELD pField);

private:
	map<const char*, CPanel*> m_pPanelMap;

private:
	virtual void Free();

};

END;