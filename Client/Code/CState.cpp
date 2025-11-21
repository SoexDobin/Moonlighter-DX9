#include "CState.h"

CState::CState()
    : m_bStarted(false), m_bCanTransit(false), m_bExited(false), m_dwCurStateKey(0), m_wsCurTex(L"<NONE>")
{
}

CState::CState(CRenderObject* pOnwer, CStateMachine* pStateMachine)
    : m_bStarted(false), m_bCanTransit(false), m_bExited(false), m_dwCurStateKey(0), m_wsCurTex(L"<NONE>")
{
}

CState::~CState()
{
}
