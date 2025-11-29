#include "pch.h"
#include "CBossShakeState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"

CBossShakeState::CBossShakeState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CBossShakeState::~CBossShakeState()
{
}

void CBossShakeState::Enter()
{
    CMonsterState::Enter();
}

void CBossShakeState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossShakeState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_SHAKE;

    m_fMinStateTime = 3.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossShakeState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CBossShakeState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossShakeState::Render_State()
{
    // WALK와 IDLE은 같은 애니메이션 사용 
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_SHAKE);
}

void CBossShakeState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::DEAD);
}

CBossShakeState* CBossShakeState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossShakeState* pIdle = new CBossShakeState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("BossShakeState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CBossShakeState::Free()
{

}
