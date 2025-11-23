#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CHouse : public CRenderObject
{
private:
    explicit CHouse(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CHouse(const CHouse& rhs);
    virtual ~CHouse() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CHouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

