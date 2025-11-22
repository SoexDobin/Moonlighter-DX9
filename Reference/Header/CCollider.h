#pragma once

#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
public:
    typedef struct tagCollision
    {
        CGameObject* pColTarget;
        COL_STATE    eColState;
    } Collision;

protected:
    explicit CCollider();
    explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCollider(const CCollider& rhs);
    virtual ~CCollider() override;

public:
    PROTOTYPE_COMPONENT         Get_ComponentType() override { return COLLIDER; }
    virtual COL_TYPE            Get_ColType() PURE;

    _bool                       Get_IsCol() const               { return m_bIsCol; }
    void                        Set_IsCol(const _bool& bIsCol)  { m_bIsCol = bIsCol; }

    const Collision&            Get_Collision() const                { return m_tCollision; }
    void                        Set_Collision(const Collision& tCol) { m_tCollision = tCol; }

public:
    void            Add_OverlapMember(CCollider* pOverlap);
    void            Release_OverlapMember(CCollider* pOverlap);
    void            Frame_Release();

#ifdef _DEBUG
    virtual void    Render_DebugCollider() PURE;
#endif

protected:
    Collision                      m_tCollision;

    _bool                          m_bIsCol;
    unordered_set<CCollider*>      m_usetOverlapCol;
    
public:
    CComponent* Clone() PURE;
protected:
    virtual void        Free();
};

END
