#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CComponent.h"
#include "CPrototypeManager.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() override;

public:
    template <typename T>
    T*                 Add_Component(COMPONENTID eID, const wstring& wsComponentKey, PROTOTYPE_COMPONENT eComponentTag);
    template <typename T>
    T*                 Add_Component(COMPONENTID eID, const wstring& wsComponentKey, const wstring& wsComponentTag);

	CComponent*		        Get_Component(COMPONENTID eID, const wstring& wsComponentKey);
    CComponent*             Get_Component(COMPONENTID eID, PROTOTYPE_COMPONENT ePrototype);
    virtual GAMEOBJECT_TYPE Get_Type() { return GAME_OBJECT; }

private:
	CComponent*		Find_Component(COMPONENTID eID, const wstring& wsComponentTag);

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float fTimeDelta);
	virtual		void		Render_GameObject();

    virtual     void        On_Collision(const Collision& tCollision) {};

protected:
	LPDIRECT3DDEVICE9							m_pGraphicDevice;
	unordered_map<wstring, CComponent*>			m_umComponent[ID_END];

protected:
	virtual void Free() override;

#pragma region Editor
public:
	virtual	void		Display_Editor();
    const TCHAR*        Get_DisplayName() { return m_szDisplayName; }
protected:
	virtual void		Add_EditorField(const char* pTag, DATATYPE type, void* pData) { m_EditorFieldList.push_back({ pTag, type, pData }); }
	virtual void		Add_EditorField(EDITORFIELD field) { m_EditorFieldList.push_back(field); }

public :
	_bool m_bDisplayInEditor;
	TCHAR m_szDisplayName[32];
	char       m_szBuffer[32];

    void    Set_EditorDisplayName(wstring wsName);

protected :
	list<EDITORFIELD> m_EditorFieldList;
#pragma endregion

  // 렌더 우선 순위
public:
    int     Get_RenderPriority() const { return m_iRenderPriority; }
    void    Set_RenderPriority(int iPriority) { m_iRenderPriority = iPriority; }

protected:
    int m_iRenderPriority = 0;

};

template <typename T>
T* CGameObject::Add_Component(COMPONENTID eID, const wstring& wsComponentKey, PROTOTYPE_COMPONENT eComponentTag)
{
    CComponent* pComponent = CPrototypeManager::GetInstance()->Clone_Prototype(eComponentTag);
    if (pComponent->Get_ComponentType() != eComponentTag)
    {
        MSG_BOX("Prototype Clone Failed");
        return nullptr;
    }

    pComponent->Set_Owner(this);
    m_umComponent[eID].insert(pair<wstring, CComponent*>(wsComponentKey, pComponent));

    return static_cast<T*>(pComponent);
}

template <typename T>
T* CGameObject::Add_Component(COMPONENTID eID, const wstring& wsComponentKey, const wstring& wsComponentTag)
{
    CComponent* pComponent = CPrototypeManager::GetInstance()->Clone_Prototype(wsComponentTag);
    if (pComponent == nullptr)
    {
        MSG_BOX("Prototype Clone Failed");
        return nullptr;
    }

    pComponent->Set_Owner(this);
    m_umComponent[eID].insert(pair<wstring, CComponent*>(wsComponentKey, pComponent));

    return static_cast<T*>(pComponent);
}

END
