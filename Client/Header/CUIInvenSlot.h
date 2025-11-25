#pragma once
#include "CRenderObject.h"

namespace Engine { class CTexture; }

class CUIInvenSlot : public CRenderObject
{
private:
    explicit CUIInvenSlot(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUIInvenSlot(const CUIInvenSlot& rhs);
    virtual ~CUIInvenSlot();

public:
    virtual			HRESULT		Ready_GameObject() override ;
    virtual			_int		Update_GameObject(const _float fTimeDelta)override;
    virtual			void		LateUpdate_GameObject(const _float fTimeDelta)override;
    virtual			void		Render_GameObject()override;

public:
    void            Set_Pos(_float fx, _float fy);

public:
    static CUIInvenSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    void                 SlotButton(); // 창 온 오프 

private:
    virtual void Free()override;
    CTexture* m_pTextureCom;

    _float      m_fWidth = 54.f;
    _float      m_fHeight = 54.f;
    _bool         m_bSVisible;


};

