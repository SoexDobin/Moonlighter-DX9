#include "pch.h"
#include "CSlimeIdleState.h"
#include "CSlimeMob.h"
#include "CSlimeStateMachine.h"

CSlimeIdleState::CSlimeIdleState(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CSlimeIdleState::~CSlimeIdleState()
{
}

void CSlimeIdleState::Enter()
{
    //CMonsterState::Enter();
}

void CSlimeIdleState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CSlimeIdleState::Ready_State()
{
    m_dwCurStateKey = CSlimeMob::SLIME_STATE::IDLE;

    m_fMinStateTime = 1.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CSlimeIdleState::Update_State(const _float& fTimeDelta)
{
    if (nullptr == m_pPlayer)
        Find_Player();

    CMonsterState::Detect_Player();

    return 0;
}

void CSlimeIdleState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CSlimeIdleState::Render_State()
{
    // IDLE과 WALK는 같은 애니메이션을 사용
    m_pOwner->Set_CurAnimKey(CSlimeMob::SLIME_STATE::IDLE);
}

void CSlimeIdleState::Determine_NextState()
{
    // TEST : IDLE -> ATK_CIRCLE -> ATK_BIG
    m_pStateMachine->Change_State(CSlimeMob::SLIME_STATE::ATK_CIRCLE);
}

CSlimeIdleState* CSlimeIdleState::Create(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
{
    CSlimeIdleState* pIdle = new CSlimeIdleState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("SlimeIdleState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CSlimeIdleState::Free()
{
}
