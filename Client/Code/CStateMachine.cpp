#include "pch.h"
#include "CStateMachine.h"

CStateMachine::CStateMachine()
    : m_pOwner(nullptr), m_pCurState(nullptr), m_dwPrevStateKey(0), m_dwCurStateKey(0)
{
}

CStateMachine::CStateMachine(CRenderObject* pOwner)
    : m_pOwner(pOwner), m_pCurState(nullptr), m_dwPrevStateKey(0), m_dwCurStateKey(0)
{
}

CStateMachine::~CStateMachine()
{
    for_each(m_vecState.begin(), m_vecState.end(), CDeleteObj());
}
