#include "pch.h"
#include "CSlimeBigState.h"
#include "CSlimeMob.h"
#include "CSlimeStateMachine.h"

CSlimeBigState::CSlimeBigState(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CSlimeBigState::~CSlimeBigState()
{
}

void CSlimeBigState::Enter()
{
    CMonsterState::Enter();
}

void CSlimeBigState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CSlimeBigState::Ready_State()
{
    m_dwCurStateKey = CSlimeMob::SLIME_STATE::ATK_BIG;

    m_fMinStateTime = 1.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CSlimeBigState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CSlimeBigState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CSlimeBigState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CSlimeMob::SLIME_STATE::ATK_BIG);
}

void CSlimeBigState::Determine_NextState()
{
    // TEST : IDLE -> ATK_Big -> ATK_BIG
    m_pStateMachine->Change_State(CSlimeMob::SLIME_STATE::IDLE);
}

CSlimeBigState* CSlimeBigState::Create(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
{
    CSlimeBigState* pBig = new CSlimeBigState(pOwner, pStateMachine);

    if (FAILED(pBig->Ready_State()))
    {
        MSG_BOX("SlimeBigState Create Failed!");
        return nullptr;
    }

    return pBig;
}

void CSlimeBigState::Free()
{
}
