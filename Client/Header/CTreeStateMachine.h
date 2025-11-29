#pragma once
#include "CStateMachine.h"

class CTreeMob;

class CTreeStateMachine : public CStateMachine
{
private:
    explicit CTreeStateMachine(CTreeMob* pOwner);
    CTreeStateMachine(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner);
    virtual ~CTreeStateMachine();

public:
    virtual HRESULT     Ready_StateMachine() override;
    virtual void               Change_State(_uint dwStateKey) override;

private:
    HRESULT                 Ready_TreeStates();

private:
    CTreeMob* m_pOwner;

public:
    static CTreeStateMachine* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner);

private:
    virtual void Free();
};

