#include "pch.h"
#include "CBossDeadState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"

CBossDeadState::CBossDeadState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CBossDeadState::~CBossDeadState()
{
}

void CBossDeadState::Enter()
{
    CMonsterState::Enter();
}

void CBossDeadState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossDeadState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::DEAD;

    m_fMinStateTime = 3.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossDeadState::Update_State(const _float& fTimeDelta)
{
    return 0;
}

void CBossDeadState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CBossDeadState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::DEAD);
}

void CBossDeadState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::WALK);
}

CBossDeadState* CBossDeadState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossDeadState* pIdle = new CBossDeadState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("BossWalkState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CBossDeadState::Free()
{

}
