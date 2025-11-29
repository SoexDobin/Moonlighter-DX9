#include "pch.h"
#include "CTreeStateMachine.h"
#include "CTreeMob.h"

#include "CMonsterState.h"
#include "CTreeAwakeState.h"
#include "CTreeIdleState.h"
#include "CTreeShakeState.h"

CTreeStateMachine::CTreeStateMachine(CTreeMob* pOwner)
    : CStateMachine(), m_pOwner(pOwner)
{
}

CTreeStateMachine::CTreeStateMachine(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner)
    : CStateMachine(pGraphicDev), m_pOwner(pOwner)
{
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

HRESULT CTreeStateMachine::Ready_TreeStates()
{
    // TODO : DEAD STATE를 만들어야 하는가?
    // T_END만큼 공간 할당 시 마지막 원소가 남게 된다

    m_vecState.resize(static_cast<int>(CTreeMob::TREE_STATE::T_END) - 1);

    CMonsterState* pState = nullptr;
    if (!(pState = CTreeAwakeState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CTreeMob::TREE_STATE::AWAKE] = pState;

    pState = nullptr;
    if (!(pState = CTreeIdleState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CTreeMob::TREE_STATE::IDLE] = pState;

    pState = nullptr;
    // 해당 상태는 Object를 생성하므로 그래픽장치를 알아야 한다
    if (!(pState = CTreeShakeState::Create(m_pGraphicDev, m_pOwner, this)))
        return E_FAIL;
    m_vecState[CTreeMob::TREE_STATE::ATK_SHAKE] = pState;
    
    return S_OK;
}

CTreeStateMachine* CTreeStateMachine::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTreeMob* pOwner)
{
    CTreeStateMachine* pStateMachine = new CTreeStateMachine(pGraphicDev, pOwner);
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
