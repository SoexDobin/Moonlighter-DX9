#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;

class CBossDeadState : public CMonsterState
{
private:
    explicit CBossDeadState(CBoss* pOwner, CBossStateMachine* pStateMachine);
    virtual ~CBossDeadState();

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual HRESULT     Ready_State() override;
    virtual _uint             Update_State(const _float& fTimeDelta) override;
    virtual void               LateUpdate_State(const _float& fTimeDelta) override;
    virtual void               Render_State() override;

private:
    void        Determine_NextState();

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;
public:
    static CBossDeadState* Create(CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

