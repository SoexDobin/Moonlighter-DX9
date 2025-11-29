#include "pch.h"
#include "CBossThrowState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"

CBossThrowState::CBossThrowState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)

{

}

CBossThrowState::CBossThrowState(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(pGraphicDev),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{
}

CBossThrowState::~CBossThrowState()
{
}

void CBossThrowState::Enter()
{
    CMonsterState::Enter();
}

void CBossThrowState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossThrowState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_THROW;

    m_fMinStateTime = 0.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossThrowState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CBossThrowState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossThrowState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_THROW);
}

void CBossThrowState::Check_EventFrame()
{
    CMonsterState::Check_EventFrame();

}

void CBossThrowState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::ATK_ROOT);
}


CBossThrowState* CBossThrowState::Create(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossThrowState* pState = new CBossThrowState(pGraphicDev, pOwner, pStateMachine);

    if (FAILED(pState->Ready_State()))
    {
        MSG_BOX("BossCuttingState Create Failed!");
        return nullptr;
    }

    return pState;
}

void CBossThrowState::Free()
{

}
