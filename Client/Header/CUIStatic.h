#pragma once
#include "CRenderObject.h"

namespace Engine { class CTexture; }

struct UIElement
{
    wstring		wstrTextureTag;
    _vec3		vPos;
    _vec3		vScale;
    Engine::CTexture* m_pTexture;
};

class CUIStatic :  public CRenderObject
{
private:
    explicit CUIStatic(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUIStatic(const CUIStatic& rhs);
    virtual ~CUIStatic();

public:
    virtual			HRESULT		Ready_GameObject()override;
    virtual			_int		Update_GameObject(const _float fTimeDelta)override;
    virtual			void		LateUpdate_GameObject(const _float fTimeDelta)override;
    virtual			void		Render_GameObject()override;

private:
    HRESULT			UI_Initialize();
    vector<UIElement>		m_VecUI;

public:
    static CUIStatic* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free()override;
};

