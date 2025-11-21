#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CState;

class CStateMachine : public CBase
{
private:
    explicit CStateMachine();
    explicit CStateMachine(CRenderObject* pOwner);
    virtual ~CStateMachine();

public:
    virtual HRESULT       Ready_StateMachine() = 0;
    virtual void                Change_State(_uint dwStateKey) = 0;

protected:
    CRenderObject* m_pOwner;
    vector<CState*> m_vecState;
    CState* m_pCurState;

    _uint                   m_dwPrevStateKey;
    _uint                   m_dwCurStateKey;

protected:
    virtual void Free();
};
