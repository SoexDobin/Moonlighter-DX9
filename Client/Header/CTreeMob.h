#pragma once

#include "CRenderObject.h"
namespace Engine {
    class CTexture;
}

class CPlayer;

class CTreeMob : public CRenderObject
{
private :
    enum TREE_STATE { T_IDLE, T_END };

private:
    explicit CTreeMob(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTreeMob(const CTreeMob& rhs);
    virtual ~CTreeMob() override;

public:
    HRESULT		Ready_GameObject() override;
    _int		Update_GameObject(const _float fTimeDelta) override;
    void		LateUpdate_GameObject(const _float fTimeDelta) override;
    void		Render_GameObject() override;

private:
    void    Configure_Component();

private:
    void    Find_Player();
    void    Chase_Player();
    CPlayer* pPlayer = nullptr;

private:
    CTexture* m_pDynamicTexCom;

public:
    static CTreeMob* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    void				Free() override;
};

