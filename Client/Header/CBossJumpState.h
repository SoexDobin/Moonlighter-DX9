#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;
class CHitRectBox;

class CBossJumpState : public CMonsterState
{
private :
    enum JUMP_TYPE { ATK, MOVE };

private:
    explicit CBossJumpState(CBoss* pOwner, CBossStateMachine* pStateMachine);
    virtual ~CBossJumpState();

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual HRESULT     Ready_State() override;
    virtual _uint             Update_State(const _float& fTimeDelta) override;
    virtual void               LateUpdate_State(const _float& fTimeDelta) override;
    virtual void               Render_State() override;

private:
    virtual void    Check_EventFrame() override;

private:
    void        Determine_NextState();
    void        Ready_Combat();

private:
    void        Jump_ToPlayer(const _float fTimeDelta);
    void        Ready_Jump();

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;

    CHitRectBox*       m_pHitBox;                             // 콜라이더와 별도의 히트박스 
    DAMAGE_INFO     m_tDamage;                            // 히트박스에 넣을 데미지 정보

private:
    _float m_fJumpDuration, m_fElapsed;

    _vec3 m_vDstPos, m_vOriginPos;

    _bool m_bReadyJump, m_bJumpingUp, m_bJumped;    // 점프 상태

public:
    static CBossJumpState* Create(CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

