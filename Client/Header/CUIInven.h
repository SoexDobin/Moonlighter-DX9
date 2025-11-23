#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTexture;
}

struct ITEM_DATA
{
    int         iID;
    wstring		IName;
};

struct ITEM_SLOT
{
    bool        bEmpty;
    ITEM_DATA   Item;
};

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

public:
    void          InvenButton();

private:
    CTexture*     m_pTextureCom;
    _bool         m_bVisible;
    void          UI_KeyInput(const _float& fTimeDelta);


public:
    static CUIInven* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free()override;
    void         Create_Slots(LPDIRECT3DDEVICE9 pGraphicDev);
    vector<ITEM_SLOT>   m_vecSlots;

};

