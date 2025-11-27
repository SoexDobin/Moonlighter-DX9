#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CBossWallSide : public CRenderObject
{
private:
    explicit CBossWallSide(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossWallSide(const CBossWallSide& rhs);
    virtual ~CBossWallSide() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CBossWallSide* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

