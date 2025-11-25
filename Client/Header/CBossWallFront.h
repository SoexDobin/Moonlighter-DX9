#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CBossWallFront : public CRenderObject
{
private:
    explicit CBossWallFront(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossWallFront(const CBossWallFront& rhs);
    virtual ~CBossWallFront() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CBossWallFront* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

