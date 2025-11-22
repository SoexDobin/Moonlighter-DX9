#include "pch.h"
#include "CTreeStateMachine.h"
#include "CTreeMob.h"

#include "CMonsterState.h"
#include "CTreeAwakeState.h"
#include "CTreeIdleState.h"
#include "CTreeShakeState.h"

CTreeStateMachine::CTreeStateMachine(CTreeMob* pOwner)
{
    m_pOwner = pOwner;
}

CTreeStateMachine::~CTreeStateMachine()
{
}

HRESULT CTreeStateMachine::Ready_StateMachine()
{
    Ready_TreeStates();

    m_pCurState = m_vecState[CTreeMob::TREE_STATE::AWAKE];
    m_pCurState->Enter();

    m_pOwner->Set_CurStateKey(CTreeMob::TREE_STATE::AWAKE, static_cast<CMonsterState*>(m_pCurState));
    return S_OK;
}

void CTreeStateMachine::Change_State(_uint dwStateKey)
{
    if (dwStateKey == m_dwCurStateKey)
        return;

    if (nullptr == m_vecState[dwStateKey] || dwStateKey >= CTreeMob::TREE_STATE::T_END)
    {
        return;
    }

    m_dwPrevStateKey = m_dwCurStateKey;
    m_dwCurStateKey = dwStateKey;

    m_pCurState->Exit();

    m_pCurState = m_vecState[m_dwCurStateKey];
    m_pCurState->Enter();

    m_pOwner->Set_CurStateKey((CTreeMob::TREE_STATE)m_dwCurStateKey, static_cast<CMonsterState*>(m_pCurState));
}

void CStateMachine::Free()
{
    for_each(m_vecState.begin(), m_vecState.end(), CDeleteObj());
    m_vecState.clear();
}

HRESULT CTreeStateMachine::Ready_TreeStates()
{
    m_vecState.resize(CTreeMob::TREE_STATE::T_END);

    CMonsterState* pState = nullptr;
    if (!(pState = CTreeAwakeState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CTreeMob::TREE_STATE::AWAKE] = pState;

    pState = nullptr;
    if (!(pState = CTreeIdleState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CTreeMob::TREE_STATE::IDLE] = pState;

    pState = nullptr;
    if (!(pState = CTreeShakeState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CTreeMob::TREE_STATE::ATK_SHAKE] = pState;
    

    return S_OK;
}

CTreeStateMachine* CTreeStateMachine::Create(CTreeMob* pOwner)
{
    CTreeStateMachine* pStateMachine = new CTreeStateMachine(pOwner);
    if (FAILED(pStateMachine->Ready_StateMachine()))
    {
        MSG_BOX("TreeStateMachine Create Failed!");
        return nullptr;
    }

    return pStateMachine;
}

void CTreeStateMachine::Free()
{
    CStateMachine::Free();
}
