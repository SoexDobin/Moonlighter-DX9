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

	void Render_Begin();
	void Render_Editor();
	void	Render_End();

	void Display_Editor(EDITORFIELD field);

public:
	CPanel* Create_Panel(const char* pName);
	CPanel* Get_Panel(const char*);
	CPanel* Find_Panel(const char* pName);

	void			Add_EditorField(const char* pName, EDITORFIELD pField);

public :
	void			Set_pTimeScale(_float* pTimeScale) { m_pTimeScale = pTimeScale; }

private :
	void			Display_MainPanel();

private:
	map<const char*, CPanel*> m_pPanelMap;

private :
    static _bool s_bEditorActive;

private :
	_bool		m_bGamePaused;
	_float		m_fFPS;

	_float*	m_pTimeScale;

	char			m_szPaused[16];
private:
	virtual void Free();

};

END;
