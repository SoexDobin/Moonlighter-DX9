#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CVineTwo : public CRenderObject
{
private:
    explicit CVineTwo(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CVineTwo(const CVineTwo& rhs);
    virtual ~CVineTwo() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CVineTwo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

