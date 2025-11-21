#include "pch.h"
#include "CStateMachine.h"

CStateMachine::CStateMachine()
    : m_pCurState(nullptr), m_dwPrevStateKey(0), m_dwCurStateKey(0)
{
}

CStateMachine::~CStateMachine()
{
    for_each(m_vecState.begin(), m_vecState.end(), CDeleteObj());
}
