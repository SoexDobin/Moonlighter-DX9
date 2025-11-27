#pragma once
#include "CRenderObject.h"

namespace Engine { class CTexture; }


class CHpBar : public CRenderObject
{
private:
    explicit CHpBar(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CHpBar(const CHpBar& rhs);
    virtual ~CHpBar();

public:
    virtual			HRESULT		Ready_GameObject()override;
    virtual			_int		Update_GameObject(const _float fTimeDelta)override;
    virtual			void		LateUpdate_GameObject(const _float fTimeDelta)override;
    virtual			void		Render_GameObject()override;

private:
    CTexture*     m_pTextureCom;
    void          Get_State(const _float& fTimeDelta);

    int           Hp_Max = 100;
    int           Hp_Min = 0;
    const float   fRPos = 209.f;
    const float   fRMax = 112.f;
    float         finter = fRMax / Hp_Max;  // 1HP
    

    int           iCount = 0;
    _bool         m_bButton = false;


public:
    static CHpBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;


};

