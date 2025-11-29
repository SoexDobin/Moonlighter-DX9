#include "pch.h"
#include "CBossRootState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"

CBossRootState::CBossRootState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CBossRootState::~CBossRootState()
{
}

void CBossRootState::Enter()
{
    CMonsterState::Enter();
}

void CBossRootState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossRootState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_ROOT;

    m_fMinStateTime = 3.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossRootState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CBossRootState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossRootState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_ROOT);
}

void CBossRootState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::ATK_SHAKE);
}

CBossRootState* CBossRootState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossRootState* pIdle = new CBossRootState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("BossRootState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CBossRootState::Free()
{

}
