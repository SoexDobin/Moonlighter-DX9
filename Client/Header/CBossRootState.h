#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;

class CBossRootState : public CMonsterState
{
private:
    explicit CBossRootState(CBoss* pOwner, CBossStateMachine* pStateMachine);
    virtual ~CBossRootState();

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
    static CBossRootState* Create(CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

