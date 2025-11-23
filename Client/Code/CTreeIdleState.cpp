#include "pch.h"
#include "CTreeIdleState.h"
#include "CTreeMob.h"
#include "CTreeStateMachine.h"

CTreeIdleState::CTreeIdleState(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CTreeIdleState::~CTreeIdleState()
{
}

void CTreeIdleState::Enter()
{
    CMonsterState::Enter();
}

void CTreeIdleState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CTreeIdleState::Ready_State()
{
    m_dwCurStateKey = CTreeMob::TREE_STATE::IDLE;

    m_fMinStateTime = 1.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CTreeIdleState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CTreeIdleState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit = m_bMinStateTimePassed)
        Determine_NextState();
}

void CTreeIdleState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CTreeMob::TREE_STATE::IDLE);
}

void CTreeIdleState::Determine_NextState()
{
    m_pStateMachine->Change_State(CTreeMob::TREE_STATE::ATK_SHAKE);
}

CTreeIdleState* CTreeIdleState::Create(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
{
    CTreeIdleState* pIdle = new CTreeIdleState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("TreeIdleState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CTreeIdleState::Free()
{
}
