#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CBossWallSideUpper : public CRenderObject
{
private:
    explicit CBossWallSideUpper(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossWallSideUpper(const CBossWallSideUpper& rhs);
    virtual ~CBossWallSideUpper() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CBossWallSideUpper* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

