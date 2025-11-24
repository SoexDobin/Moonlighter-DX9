#pragma once

#include "CCollider.h"

BEGIN(Engine)

class ENGINE_DLL CRectCollider : public CCollider
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
    const _vec3&        Get_Dimension() const                       { return m_vDimension; }
    void                Set_Dimension(const _vec3& vDimension)      { m_vDimension = vDimension; }

public:
    HRESULT                 Ready_RectCollider();
    virtual _int            Update_Component(const _float fTimeDelta) override;
    virtual void            LateUpdate_Component() override;
    virtual _bool           Check_Collision(CCollider* pCol) override;

#ifdef _DEBUG
    virtual void            Render_DebugCollider() override;
#endif

private:
    _vec3                 m_vDimension; 

public:
    static CRectCollider*   Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent*     Clone() override;

private:
    virtual void            Free() override;
};

END
