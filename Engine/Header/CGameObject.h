#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() override;

public:
	CComponent*		        Get_Component(COMPONENTID eID, const wstring& wsComponentTag);
    virtual GAMEOBJECT_TYPE Get_Type() { return GAME_OBJECT; }

private:
	CComponent*		Find_Component(COMPONENTID eID, const wstring& wsComponentTag);

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float fTimeDelta);
	virtual		void		Render_GameObject();

protected:
	LPDIRECT3DDEVICE9							m_pGraphicDevice;
	unordered_map<wstring, CComponent*>			m_umComponent[ID_END];

protected:
	virtual void Free() override;

#pragma region Editor
public:
	virtual			void		Display_Editor();

protected:
	virtual void		Add_EditorField(const char* pTag, DATATYPE type, void* pData) { m_EditorFieldList.push_back({ pTag, type, pData }); }
	virtual void		Add_EditorField(EDITORFIELD field) { m_EditorFieldList.push_back(field); }

public :
	_bool m_bDisplayInEditor;
	TCHAR m_szDisplayName[32];
	char       m_szBuffer[32];

protected :
	list<EDITORFIELD> m_EditorFieldList;
#pragma endregion

};

END
