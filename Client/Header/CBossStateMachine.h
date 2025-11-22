#pragma once
#include "CStateMachine.h"

class CBoss;

class CBossStateMachine :  public CStateMachine
{
private :
    explicit CBossStateMachine(CBoss* pOwner);
    virtual ~CBossStateMachine();

public :
    virtual HRESULT     Ready_StateMachine();
    virtual void               Change_State(_uint dwStateKey);

private :
    HRESULT                 Ready_BossStates();

private:
    CBoss* m_pOwner;

public :
    static CBossStateMachine* Create(CBoss* pOwner);

private :
    virtual void Free();
};

