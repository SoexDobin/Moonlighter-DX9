#pragma region INCLUDE
#include "pch.h"
#include "CBossWalkState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"
#include "CStatComponent.h"
#pragma endregion


CBossWalkState::CBossWalkState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CBossWalkState::~CBossWalkState()
{
}

void CBossWalkState::Enter()
{
    CMonsterState::Enter();
}

void CBossWalkState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossWalkState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::WALK;

    m_fMinStateTime = 3.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossWalkState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();
    Move_ToPlayer(fTimeDelta);

    return 0;
}

void CBossWalkState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CBossWalkState::Render_State()
{
    // WALK와 IDLE은 같은 애니메이션 사용 
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_JUMP);
}

void CBossWalkState::Determine_NextState()
{
    //m_pStateMachine->Change_State(CBoss::BOSS_STATE::IDLE);
}

void CBossWalkState::Move_ToPlayer(const _float fTimeDelta)
{
    m_pOwnerTransformCom->Move_Pos(&m_vDirToPlayer, fTimeDelta, m_pOwner->Get_SpeedStat()->Get_CurValue());
}

CBossWalkState* CBossWalkState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossWalkState* pIdle = new CBossWalkState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("BossWalkState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CBossWalkState::Free()
{

}
