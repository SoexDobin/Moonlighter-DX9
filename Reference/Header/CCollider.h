#pragma once

#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
protected:
    explicit CCollider();
    explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCollider(const CCollider& rhs);
    virtual ~CCollider() override;

public:
    PROTOTYPE_COMPONENT Get_ComponentType() override { return COLLIDER; }

protected:
    _bool       m_bIsCol;

public:
    CComponent* Clone() PURE;
protected:
    virtual void        Free();
};

END
