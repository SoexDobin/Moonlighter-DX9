#pragma once
#include "CMonsterState.h"

class CBoss;
class CBossStateMachine;

class CBossAwakeState : public CMonsterState
{
private:
    explicit CBossAwakeState(CBoss* pOwner, CBossStateMachine* pStateMachine);
    virtual ~CBossAwakeState();

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual HRESULT     Ready_State() override;
    virtual _uint             Update_State(const _float& fTimeDelta) override;
    virtual void               LateUpdate_State(const _float& fTimeDelta) override;
    virtual void               Render_State() override;

private :
    virtual void    Check_EventFrame() override;

private:
    void        Determine_NextState();

private:
    void        Move_ToPlayer(const _float fTimeDelta);

private:
    CBoss* m_pOwner;
    CBossStateMachine* m_pStateMachine;
public:
    static CBossAwakeState* Create(CBoss* pOwner, CBossStateMachine* pStateMachine);

private:
    virtual void Free();


};

