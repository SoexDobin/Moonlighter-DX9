#pragma once

#include "CComponent.h"

BEGIN(Engine)

typedef struct tagCollision
{
    friend class CCollider;
    CGameObject*    pColTarget;     // 충돌 당한 오브젝트
    CCollider*      pColSource;     // 충돌 당한 오브젝트의 콜라이더
    COL_STATE       eColState;      // 충돌 상태 첫 충돌, 충돌중, 충돌 종료

} Collision;

class ENGINE_DLL CCollider : public CComponent
{
protected:
    explicit CCollider();
    explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCollider(const CCollider& rhs);
    virtual ~CCollider() override;

public:
    virtual void        On_Enable() override {
        m_bEnable = true;
        m_bIsCol = false;
        m_eState = COL_STATE_END;
        m_tCollision = { nullptr, nullptr, COL_STATE_END };

        m_usetOverlapCol.clear();
    };
    virtual void        On_Disable() override {
        m_bEnable = false;
        m_bIsCol = false;
        m_eState = COL_STATE_END;
        m_tCollision = { nullptr, nullptr, COL_STATE_END };

        m_usetOverlapCol.clear();
    };

public:
    virtual PROTOTYPE_COMPONENT Get_ComponentType() override { return COLLIDER; }
    virtual COL_TYPE            Get_ColType() PURE;

    _bool                       Get_IsCol() const                    { return m_bIsCol; }
    void                        Set_IsCol(const _bool& bIsCol)       { m_bIsCol = bIsCol; }
    const Collision&            Get_Collision() const                { return m_tCollision; }
    void                        Set_Collision(const Collision& tCol) { m_tCollision = tCol; }
    COL_STATE                   Get_ColState() const                 { return m_eState; }
    void                        Set_ColState(COL_STATE eState)       { m_eState = eState; }

    const _vec3&                Get_Offset() const                   { return m_vOffset; }
    void                        Set_Offset(const _vec3& vOffset)     { m_vOffset = vOffset; }
    _float                      Get_Scale() const                    { return m_fScale; }
    void                        Set_Scale(const _float fScale)       { m_fScale = fScale; }

public:
    _bool                       Is_Overlapped(CCollider* pOverlap);
    void                        Add_OverlapMember(CCollider* pOverlap);
    void                        Release_OverlapMember(CCollider* pOverlap);

public:
    virtual _bool               Check_Collision(CCollider* pCol) PURE;
#ifdef _DEBUG
    virtual void                Render_DebugCollider() PURE;
#endif

protected:
    Collision                      m_tCollision;
    COL_STATE                      m_eState;
    _bool                          m_bIsCol;
    unordered_set<CCollider*>      m_usetOverlapCol;

    _vec3                          m_vOffset;
    _float                         m_fScale = 1.f; 
    
public:
    CComponent* Clone() PURE;
protected:
    virtual void        Free();
#pragma region Editor
#ifdef _DEBUG
public:
    virtual void Display_Editor(const char* pObjTag) override;
protected:
    _bool m_bShowOverlapsInEditor = false;
#endif
#pragma endregion
};

END
