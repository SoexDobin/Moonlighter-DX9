#pragma once

#include "CRenderObject.h"
namespace Engine {
    class CTexture;
}

class CPlayer;

class CBoss : public CRenderObject
{
private:
    enum BOSS_STATE { B_IDLE, B_WALK, B_END };
private:
    explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBoss(const CBoss& rhs);
    virtual ~CBoss() override;

public:
    HRESULT		Ready_GameObject() override;
    _int		Update_GameObject(const _float fTimeDelta) override;
    void		LateUpdate_GameObject(const _float fTimeDelta) override;
    void		Render_GameObject() override;

private :
    void    Configure_Component();

private :
    void    Find_Player();
    void    Chase_Player();
    CPlayer* pPlayer = nullptr;

private:
    CTexture* m_pDynamicTexCom;

public:
    static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    void				Free() override;
};

