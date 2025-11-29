#include "pch.h"
#include "CSlimeCircleState.h"
#include "CSlimeMob.h"
#include "CSlimeStateMachine.h"

CSlimeCircleState::CSlimeCircleState(CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)

{

}

CSlimeCircleState::CSlimeCircleState(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
    : CMonsterState(pGraphicDev),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{
}

CSlimeCircleState::~CSlimeCircleState()
{
}

void CSlimeCircleState::Enter()
{
    CMonsterState::Enter();
}

void CSlimeCircleState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CSlimeCircleState::Ready_State()
{
    m_dwCurStateKey = CSlimeMob::SLIME_STATE::ATK_CIRCLE;

    m_fMinStateTime = 1.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CSlimeCircleState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CSlimeCircleState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CSlimeCircleState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CSlimeMob::SLIME_STATE::ATK_CIRCLE);
}

void CSlimeCircleState::Determine_NextState()
{
    // TEST : IDLE -> ATK_CIRCLE -> ATK_BIG
    m_pStateMachine->Change_State(CSlimeMob::SLIME_STATE::ATK_BIG);
}

CSlimeCircleState* CSlimeCircleState::Create(LPDIRECT3DDEVICE9 pGraphicDev, CSlimeMob* pOwner, CSlimeStateMachine* pStateMachine)
{
    CSlimeCircleState* pCircle = new CSlimeCircleState(pGraphicDev, pOwner, pStateMachine);

    if (FAILED(pCircle->Ready_State()))
    {
        MSG_BOX("SlimeCircleState Create Failed!");
        return nullptr;
    }

    return pCircle;
}

void CSlimeCircleState::Free()
{
}
