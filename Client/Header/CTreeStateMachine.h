#pragma once
#include "CStateMachine.h"

class CTreeMob;

class CTreeStateMachine : public CStateMachine
{
private:
    explicit CTreeStateMachine(CTreeMob* pOwner);
    virtual ~CTreeStateMachine();

public:
    virtual HRESULT     Ready_StateMachine() override;
    virtual void               Change_State(_uint dwStateKey) override;

private:
    HRESULT                 Ready_TreeStates();

private:
    CTreeMob* m_pOwner;

public:
    static CTreeStateMachine* Create(CTreeMob* pOwner);

private:
    virtual void Free();
};

