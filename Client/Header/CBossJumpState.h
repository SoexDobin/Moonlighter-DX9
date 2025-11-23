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
    void        Compute_DeltaY();

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;

private:
    _bool m_bJumping, m_bJumpCompleted;
    _float m_fDeltaX, m_fDeltaY, m_fDeltaZ;
    _float m_fJumpDuration;
    _float m_fOffsetY;
    _float m_fDiffX, m_fDiffZ;

    _vec3 m_vDstPos;

public:
    static CBossJumpState* Create(CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

