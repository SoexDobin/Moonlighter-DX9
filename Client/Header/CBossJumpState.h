#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;

class CBossJumpState : public CMonsterState
{
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

private:
    void        Jump_ToPlayer(const _float fTimeDelta);
    void        Ready_Jump();
    void        Jumping(const _float fTimeDelta);
    void        Finish_Jump();

    void        Compute_DeltaY();

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;

private:
    _vec3 m_vDeltaMove;
    _float m_fJumpDuration, m_fElapsed;
    _float m_fOffsetY;
    _float m_fDiffX, m_fDiffZ;

    _vec3 m_vDstPos, m_vOriginPos;

    _bool m_bReadyJump, m_bJumpingUp, m_bJumpingDown, m_bJumped;

public:
    static CBossJumpState* Create(CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

