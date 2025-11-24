#pragma once
#include "CRenderObject.h"

namespace Engine {
    class CTexture;
}

class CPlayer : public CRenderObject
{
private:
    enum WEAPON { NONE, SPEAR, BOW, WEAPON_END };
    enum STATE { IDLE, WALK, ROLL, SPEAR_COMBO, SPEAR_CHARGE, BOW_NORMAL, BOW_CHARGE, SWIM, STATE_END };
    enum DIRECTION { DIR_DOWN, DIR_UP, DIR_LEFT, DIR_RIGHT, DIR_END };

private:
    explicit    CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit    CPlayer(const CPlayer& rhs);
    virtual     ~CPlayer() override;

public:
    HRESULT     Ready_GameObject() override;
    _int        Update_GameObject(const _float fTimeDelta) override;
    void        LateUpdate_GameObject(const _float fTimeDelta) override;
    void        Render_GameObject() override;

private:
    HRESULT     Ready_Animation();
    _uint       Get_AnimationIndex();
    void        Save_Dir();
    void	    Key_Input(const _float& fTimeDelta);

private:
    CTexture* m_pTexCom;

    WEAPON      m_eWeapon;
    STATE       m_eState;
    DIRECTION   m_eDir;

    STATE       m_ePrevState;
    DIRECTION   m_ePrevDir;

    _float      m_fActionDuration;
    _float      m_fActionTime;
    _vec3       m_vActionDir;

public:
    static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    void                Free() override;
};
