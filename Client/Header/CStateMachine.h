#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CState;

class CStateMachine : public CBase
{
protected:
    explicit CStateMachine();
    explicit CStateMachine(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CStateMachine();

public:
    virtual HRESULT       Ready_StateMachine() = 0;
    virtual void                Change_State(_uint dwStateKey) = 0;

protected:
    vector<CState*> m_vecState;
    CState* m_pCurState;

    _uint                   m_dwPrevStateKey;
    _uint                   m_dwCurStateKey;

    LPDIRECT3DDEVICE9   m_pGraphicDev;

protected:
    virtual void Free();
};
