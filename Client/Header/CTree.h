#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CTree : public CRenderObject
{
private:
    explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTree(const CTree& rhs);
    virtual ~CTree() override;

    CTexture* m_pTextureCom;

    void Free() override;
public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float fTimeDelta) override;
    void LateUpdate_GameObject(const _float fTimeDelta) override;
    void Render_GameObject() override;

    static CTree* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

