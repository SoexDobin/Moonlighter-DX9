#pragma once
#include "CStateMachine.h"

class CSlimeMob;

class CSlimeStateMachine : public CStateMachine
{
private:
    explicit CSlimeStateMachine(CSlimeMob* pOwner);
    explicit CSlimeStateMachine(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner);
    virtual ~CSlimeStateMachine();

public:
    virtual HRESULT     Ready_StateMachine() override;
    virtual void               Change_State(_uint dwStateKey) override;

private:
    HRESULT                 Ready_SlimeStates();

private:
    CSlimeMob* m_pOwner;

public:
    static CSlimeStateMachine* Create(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner);

private:
    virtual void Free();
};

