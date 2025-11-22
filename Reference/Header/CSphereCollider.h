#pragma once

#include "CCollider.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CCollider
{
private:
    explicit CSphereCollider();
    explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CSphereCollider(const CSphereCollider& rhs);
    virtual ~CSphereCollider() override;

public:
    COL_TYPE        Get_ColType() override { return SPHERE_COL; }

public:
    HRESULT         Ready_SphereCollider();
    virtual _bool   Check_Collision(CCollider* pCol) override;

#ifdef _DEBUG
    virtual void    Render_DebugCollider() override;
#endif
public:
    static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent*     Clone() override;

protected:
    virtual void Free() override;
};

END
