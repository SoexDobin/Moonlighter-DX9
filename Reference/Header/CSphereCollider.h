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
    PROTOTYPE_COMPONENT Get_ComponentType() override { return SPHERE_COLLIDER; }
    COL_TYPE            Get_ColType() override { return SPHERE_COL; }

    _float          Get_Radius() const { return m_fRadius; }
    void            Set_Radius(const _float fRadius) { m_fRadius = fRadius; }

public:
    HRESULT         Ready_SphereCollider();
    virtual _bool   Check_Collision(CCollider* pCol) override;

    virtual _int    Update_Component(const _float fTimeDelta) override;
    virtual void    LateUpdate_Component() override;

#ifdef _DEBUG
    virtual void    Render_DebugCollider() override;
#endif

private:
    _float          m_fRadius;

public:
    static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent*     Clone() override;

protected:
    virtual void Free() override;

#pragma region Editor
#ifdef _DEBUG

public:
    virtual void Display_Editor(const char* pObjTag) override;
#endif
#pragma endregion
};

END
