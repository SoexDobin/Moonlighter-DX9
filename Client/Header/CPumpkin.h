#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
    class CRectCollider;
}

class CPumpkin : public CRenderObject
{
private:
    explicit CPumpkin(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CPumpkin(const CPumpkin& rhs);
    virtual ~CPumpkin() override;

    CTexture* m_pTextureCom;
    CRectCollider* m_pRectCollider;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;
    void On_Collision(const Collision& tCollision) override;

    static CPumpkin* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

