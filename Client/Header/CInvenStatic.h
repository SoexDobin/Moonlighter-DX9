#pragma once
#include "CRenderObject.h"

namespace Engine { class CTexture; }

class CInvenStatic : public CRenderObject
{
private:
    explicit CInvenStatic(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CInvenStatic(const CInvenStatic& rhs);
    virtual ~CInvenStatic();

public:
    virtual			HRESULT		Ready_GameObject()override;
    virtual			_int		Update_GameObject(const _float fTimeDelta)override;
    virtual			void		LateUpdate_GameObject(const _float fTimeDelta)override;
    virtual			void		Render_GameObject()override;

public:
    void          InvenStatic();

private:
    CTexture*     m_pTextureCom;
    _bool         m_bInVisible; // 창 켜짐 여부

    void          UI_KeyInput(const _float& fTimeDelta);


public:
    static CInvenStatic* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;

};

