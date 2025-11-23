#include "pch.h"
#include "CBossStateMachine.h"
#include "CBoss.h"

#include "CBossAwakeState.h"
#include "CBossIdleState.h"
#include "CBossWalkState.h"
#include "CBossJumpState.h"
#include "CBossThrowState.h"
#include "CBossShakeState.h"
#include "CBossRootState.h"
#include "CBossDeadState.h"


CBossStateMachine::CBossStateMachine(CBoss* pOwner)
{
    m_pOwner = pOwner;
}

CBossStateMachine::~CBossStateMachine()
{
}

HRESULT CBossStateMachine::Ready_StateMachine()
{
    Ready_BossStates();

    m_pCurState = m_vecState[CBoss::BOSS_STATE::AWAKE];
    m_pCurState->Enter();

    m_pOwner->Set_CurStateKey(CBoss::BOSS_STATE::AWAKE, static_cast<CMonsterState*>(m_pCurState));
    return S_OK;
}

void CBossStateMachine::Change_State(_uint dwStateKey)
{
    if (dwStateKey == m_dwCurStateKey)
        return;

    if (nullptr == m_vecState[dwStateKey] || dwStateKey >= CBoss::BOSS_STATE::B_END)
    {
        return;
    }

    m_dwPrevStateKey = m_dwCurStateKey;
    m_dwCurStateKey = dwStateKey;

    m_pCurState->Exit();

    m_pCurState = m_vecState[m_dwCurStateKey];
    m_pCurState->Enter();

    m_pOwner->Set_CurStateKey((CBoss::BOSS_STATE)m_dwCurStateKey, static_cast<CMonsterState*>(m_pCurState));
}

void CBossStateMachine::Free()
{
    for_each(m_vecState.begin(), m_vecState.end(), CDeleteObj());
    m_vecState.clear();
}

HRESULT CBossStateMachine::Ready_BossStates()
{
    m_vecState.resize(CBoss::BOSS_STATE::B_END);

    CMonsterState* pState = nullptr;

    if (!(pState = CBossAwakeState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::AWAKE] = pState;

    if (!(pState = CBossIdleState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::IDLE] = pState;

    if (!(pState = CBossJumpState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::ATK_JUMP] = pState;

    if (!(pState = CBossThrowState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::ATK_THROW] = pState;

    if (!(pState = CBossRootState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::ATK_ROOT] = pState;

    if (!(pState = CBossShakeState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::ATK_SHAKE] = pState;

    if (!(pState = CBossDeadState::Create(m_pOwner, this)))
        return E_FAIL;
    m_vecState[CBoss::BOSS_STATE::DEAD] = pState;

    if (!(pState = CBossWalkState::Create(m_pOwner, this)))
        return E_FAIL;    
    m_vecState[CBoss::BOSS_STATE::WALK] = pState;

    return S_OK;
}

CBossStateMachine* CBossStateMachine::Create(CBoss* pOwner)
{
    CBossStateMachine* pStateMachine = new CBossStateMachine(pOwner);
    if (FAILED(pStateMachine->Ready_StateMachine()))
    {
        MSG_BOX("BossStateMachine Create Failed!");
        return nullptr;
    }

    return pStateMachine;
}
