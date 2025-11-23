#include "pch.h"
#include "CStateMachine.h"
#include "CState.h"

CStateMachine::CStateMachine()
    : m_pCurState(nullptr), m_dwPrevStateKey(0), m_dwCurStateKey(0)
{
}

CStateMachine::~CStateMachine()
{

}

void CStateMachine::Free()
{
    for_each(m_vecState.begin(), m_vecState.end(), CDeleteObj());
    m_vecState.clear();
}
