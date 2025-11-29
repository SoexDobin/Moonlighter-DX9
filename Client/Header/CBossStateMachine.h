#pragma once
#include "CStateMachine.h"

class CBoss;

class CBossStateMachine :  public CStateMachine
{
private :
    explicit CBossStateMachine(CBoss* pOwner);
    explicit CBossStateMachine(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner);
    virtual ~CBossStateMachine();

public :
    virtual HRESULT     Ready_StateMachine();
    virtual void               Change_State(_uint dwStateKey);

private :
    HRESULT                 Ready_BossStates();

private:
    CBoss* m_pOwner;

public :
    static CBossStateMachine* Create(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner);

private :
    virtual void Free();
};

