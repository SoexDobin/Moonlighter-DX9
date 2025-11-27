#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CVineOne : public CRenderObject
{
private:
    explicit CVineOne(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CVineOne(const CVineOne& rhs);
    virtual ~CVineOne() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CVineOne* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

