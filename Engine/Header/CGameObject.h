#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CComponent.h"
#include "CPrototypeManager.h"
#include "TagLayerMask.h"

BEGIN(Engine)

class CManagement;

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() override;

public:
    template <typename T> // 엔진 컴포넌트 추가 
    T*                 Add_Component(COMPONENTID eID,                       // 정적, 동적 컴포넌트 지정
                                     const wstring& wsComponentKey,         // 오브젝트에 등록할 컴포넌트 키
                                     PROTOTYPE_COMPONENT eComponentTag);    // 엔진 컴포넌트 enum 태그
    template <typename T> // 클라이언트 사용자 정의 컴포넌트 추가
    T*                 Add_Component(COMPONENTID eID,                       // 정적, 동적 컴포넌트 지정
                                     const wstring& wsComponentKey,         // 오브젝트에 등록할 컴포넌트 키
                                     const wstring& wsComponentTag);        // 클라이언트에 지정한 wstring 태그
public:
    static void        Destroy(CGameObject* pObj) { pObj->Set_Destroy(); }  // 파괴할 오브젝트로 변경 다음 업데이트에 제거
    static void        DontDestroySceneLoad(CGameObject* pObj);             // 해당 오브젝트를 캐시 등록하여 다음씬에 재사용
    
public:
    void               Init_Layer(const wstring& wLayerTag, const wstring& wObjectKey);     // Add_GameObject시 레이어 마스크 초기화
    const LayerMask&   Get_Object_LayerMask();                                              // public const 레이어 마스크 제공
protected:
    LayerMask&         Get_LayerMask();                                                     // 오브젝트에서 레이어바스크 제공
    
public:
	CComponent*		        Get_Component(COMPONENTID eID, const wstring& wsComponentKey);      // 클라이언트 사용자 정의 컴포넌트 검색  // 동일 이름이면 첫번째만
    CComponent*             Get_Component(COMPONENTID eID, PROTOTYPE_COMPONENT ePrototype);     // 엔진 컴포넌트 검색                  // 동일 이름이면 첫번째만
    virtual GAMEOBJECT_TYPE Get_Type() { return GAME_OBJECT; }

private:
	CComponent*		Find_Component(COMPONENTID eID, const wstring& wsComponentTag);

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float fTimeDelta);
    virtual		void		Render_GameObject() { };
    virtual     void        On_Collision(const Collision& tCollision) { };

private:
    void                    Set_Destroy() { m_bIsDestroy = TRUE; }                          // 게임 오브젝트 자체에서만 사용 가능

protected:
	LPDIRECT3DDEVICE9							m_pGraphicDevice;
	unordered_map<wstring, CComponent*>			m_umComponent[ID_END];
    LayerMask                                   m_tLayerMask;                               // 레이어 지정에 사용할 구조체

private:
    _bool										m_bIsDestroy;                               // private 함수 호출을 통해 변경

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

    int     Get_ObjectID() { return m_iObjectID; }

protected:
    int m_iRenderPriority = 0;
    int m_iObjectID = 0;
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
