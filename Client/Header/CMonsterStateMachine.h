#pragma once
#include "CStateMachine.h"
class CMonsterStateMachine :  public CStateMachine
{
protected:
    explicit CMonsterStateMachine();
    virtual ~CMonsterStateMachine();

public:
    virtual HRESULT       Ready_StateMachine();
    virtual void                Change_State(_uint dwStateKey);




};

