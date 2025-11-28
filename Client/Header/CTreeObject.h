#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CTreeObject : public CRenderObject
{
private:
    explicit CTreeObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTreeObject(const CTreeObject& rhs);
    virtual ~CTreeObject() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CTreeObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

