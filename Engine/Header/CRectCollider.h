#pragma once

#include "CCollider.h"

class CRectCollider : public CCollider
{
private:
    explicit CRectCollider();
    explicit CRectCollider(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRectCollider(const CRectCollider& rhs);
    virtual ~CRectCollider() override;

public:
    PROTOTYPE_COMPONENT     Get_ComponentType() override    { return RECT_COLLIDER; }
    COL_TYPE                Get_ColType() override          { return RECT_COL; }

public:
    HRESULT                 Ready_RectCollider();
    virtual _int            Update_Component(const _float fTimeDelta) override;
    virtual void            LateUpdate_Component() override;
    virtual _bool           Check_Collision(CCollider* pCol) override;

#ifdef _DEBUG
    virtual void            Render_DebugCollider() override;
#endif

public:
    static CRectCollider*   Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent*     Clone() override;

private:
    virtual void            Free() override;
};

