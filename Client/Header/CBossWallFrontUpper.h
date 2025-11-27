#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CBossWallFrontUpper : public CRenderObject
{
private:
    explicit CBossWallFrontUpper(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossWallFrontUpper(const CBossWallFrontUpper& rhs);
    virtual ~CBossWallFrontUpper() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CBossWallFrontUpper* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

