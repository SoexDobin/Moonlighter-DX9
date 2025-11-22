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
