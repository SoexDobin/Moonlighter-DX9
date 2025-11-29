#include "pch.h"
#include "CSlimeStateMachine.h"
#include "CSlimeMob.h"

#include "CMonsterState.h"
#include "CSlimeIdleState.h"
#include "CSlimeWalkState.h"
#include "CSlimeCircleState.h"
#include "CSlimeBigState.h"

CSlimeStateMachine::CSlimeStateMachine(CSlimeMob* pOwner)
    : CStateMachine(), m_pOwner(pOwner)
{
}

CSlimeStateMachine::CSlimeStateMachine(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner)
    : CStateMachine(pGraphicDev), m_pOwner(pOwner)
{
}

CSlimeStateMachine::~CSlimeStateMachine()
{
}

HRESULT CSlimeStateMachine::Ready_StateMachine()
{
    Ready_SlimeStates();

    m_pCurState = m_vecState[CSlimeMob::SLIME_STATE::IDLE];
    m_pCurState->Enter();

    m_pOwner->Set_CurStateKey(CSlimeMob::SLIME_STATE::IDLE, static_cast<CMonsterState*>(m_pCurState));
    return S_OK;
}

void CSlimeStateMachine::Change_State(_uint dwStateKey)
{
    if (dwStateKey == m_dwCurStateKey)
        return;

    if (nullptr == m_vecState[dwStateKey] || dwStateKey >= CSlimeMob::SLIME_STATE::S_END)
    {
        return;
    }

    m_dwPrevStateKey = m_dwCurStateKey;
    m_dwCurStateKey = dwStateKey;

    m_pCurState->Exit();

    m_pCurState = m_vecState[m_dwCurStateKey];
    m_pCurState->Enter();

    m_pOwner->Set_CurStateKey((CSlimeMob::SLIME_STATE)m_dwCurStateKey, static_cast<CMonsterState*>(m_pCurState));
}

HRESULT CSlimeStateMachine::Ready_SlimeStates()
{
    // TODO : DEAD STATE를 만들어야 하는가?
    // T_END만큼 공간 할당 시 마지막 원소가 남게 된다

    m_vecState.resize(static_cast<int>(CSlimeMob::SLIME_STATE::S_END) - 1);

    CMonsterState* pState = nullptr;
    if (!(pState = CSlimeIdleState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CSlimeMob::SLIME_STATE::IDLE] = pState;

    pState = nullptr;
    if (!(pState = CSlimeCircleState::Create(m_pGraphicDev, m_pOwner, this)))
        return E_FAIL;
    m_vecState[CSlimeMob::SLIME_STATE::ATK_CIRCLE] = pState;

    pState = nullptr;
    if (!(pState = CSlimeBigState::Create(m_pGraphicDev, m_pOwner, this)))
        return E_FAIL;
    m_vecState[CSlimeMob::SLIME_STATE::ATK_BIG] = pState;

    pState = nullptr;
    if (!(pState = CSlimeWalkState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CSlimeMob::SLIME_STATE::WALK] = pState;

    return S_OK;
}

CSlimeStateMachine* CSlimeStateMachine::Create(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner)
{
    CSlimeStateMachine* pStateMachine = new CSlimeStateMachine(pGraphicDev, pOwner);
    if (FAILED(pStateMachine->Ready_StateMachine()))
    {
        MSG_BOX("SlimeStateMachine Create Failed!");
        return nullptr;
    }

    return pStateMachine;
}

void CSlimeStateMachine::Free()
{
    CStateMachine::Free();
}
