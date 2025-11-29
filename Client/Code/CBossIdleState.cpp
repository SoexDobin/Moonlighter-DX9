#include "pch.h"
#include "CBossIdleState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"

CBossIdleState::CBossIdleState(CBoss* pOwner, CBossStateMachine* pStateMachine)
   : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CBossIdleState::~CBossIdleState()
{
}

void CBossIdleState::Enter()
{
    CMonsterState::Enter();
}

void CBossIdleState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossIdleState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::IDLE;

    m_fMinStateTime = 2.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossIdleState::Update_State(const _float& fTimeDelta)
{
    return 0;
}

void CBossIdleState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CBossIdleState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::IDLE);
}

void CBossIdleState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::ATK_ROOT);
}

CBossIdleState* CBossIdleState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossIdleState* pIdle = new CBossIdleState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("BossIdleState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CBossIdleState::Free()
{

}
