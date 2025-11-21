#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

class CUIInven : public CRenderObject
{
private:
    explicit CUIInven(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUIInven(const CUIInven& rhs);
    virtual ~CUIInven();

public:
    virtual			HRESULT		Ready_GameObject()override;
    virtual			_int		Update_GameObject(const _float fTimeDelta)override;
    virtual			void		LateUpdate_GameObject(const _float fTimeDelta)override;
    virtual			void		Render_GameObject()override;

private:
    CTexture*     m_pTextureCom;
    

public:
    static CUIInven* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free()override;

};

