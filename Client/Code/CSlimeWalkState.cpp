#include "pch.h"
#include "CSlimeWalkState.h"
#include "CSlimeMob.h"
#include "CSlimeStateMachine.h"

CSlimeWalkState::CSlimeWalkState(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CSlimeWalkState::~CSlimeWalkState()
{
}

void CSlimeWalkState::Enter()
{
    CMonsterState::Enter();
}

void CSlimeWalkState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CSlimeWalkState::Ready_State()
{
    m_dwCurStateKey = CSlimeMob::SLIME_STATE::WALK;

    m_fMinStateTime = 1.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CSlimeWalkState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CSlimeWalkState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CSlimeWalkState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CSlimeMob::SLIME_STATE::IDLE);
}

void CSlimeWalkState::Determine_NextState()
{
    m_pStateMachine->Change_State(CSlimeMob::SLIME_STATE::ATK_CIRCLE);
}

CSlimeWalkState* CSlimeWalkState::Create(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
{
    CSlimeWalkState* pWalk = new CSlimeWalkState(pOwner, pStateMachine);

    if (FAILED(pWalk->Ready_State()))
    {
        MSG_BOX("SlimeWalkState Create Failed!");
        return nullptr;
    }

    return pWalk;
}

void CSlimeWalkState::Free()
{
}
