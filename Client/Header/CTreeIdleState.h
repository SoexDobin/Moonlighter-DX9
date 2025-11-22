#pragma once
#include "CMonsterState.h"

class CTreeMob;
class CTreeStateMachine;

class CTreeIdleState : public CMonsterState
{
private:
    explicit CTreeIdleState(CTreeMob* pOwner, CTreeStateMachine* pStateMachine);
    virtual ~CTreeIdleState();

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
    CTreeMob* m_pOwner;
    CTreeStateMachine* m_pStateMachine;
public:
    static CTreeIdleState* Create(CTreeMob* pOwner, CTreeStateMachine* pStateMachine);

private:
    virtual void Free();


};

