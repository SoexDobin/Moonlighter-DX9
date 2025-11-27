#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CDungeonWall : public CRenderObject
{
private:
    explicit CDungeonWall(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDungeonWall(const CDungeonWall& rhs);
    virtual ~CDungeonWall() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CDungeonWall* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

