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
    void        On_Collision(const Collision& tCollision) override;

private:
    HRESULT     Ready_Animation();
    _uint       Get_AnimationIndex();
    void        Save_Dir();
    void	    Key_Input(const _float& fTimeDelta);
    bool        IsBusy() const;

private:
    void        BowNormal_BackStep();
    void        BowNormal_Fire();
    void        BowCharge_Fire();

    void        SpearComboAttackStart(_uint iStep);
    void        SpearComboAttackEnd();
    void        SpearCombo_FirstHit();
    void        SpearCombo_SecondHit();
    void        SpearCombo_ThirdHit();

    void        SpearCharge_FirstHit();
    void        SpearCharge_SecondHit();

private:
    CTexture*       m_pTexCom;
    CRectCollider*  m_pColCom;
    WEAPON          m_eWeapon;
    STATE           m_eState;
    DIRECTION       m_eDir;

    STATE           m_ePrevState;
    DIRECTION       m_ePrevDir;

    _vec3           m_vActionDir;

    _float          m_fRollDuration;
    _float          m_fRollTime;

    _uint           m_iPrevFrame;
    _uint           m_iCurFrame;

    bool            m_bBowBackStep;
    _float          m_fBowBackStepTimer;

    bool            m_bBowChargeFired;

    _uint           m_iComboStep;
    bool            m_bComboCheck;

    bool            m_bFPrev;

public:
    static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    void                Free() override;
};
