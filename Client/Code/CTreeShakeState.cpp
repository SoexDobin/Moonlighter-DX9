#include "pch.h"
#include "CTreeShakeState.h"
#include "CTreeMob.h"
#include "CTreeStateMachine.h"

CTreeShakeState::CTreeShakeState(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CTreeShakeState::~CTreeShakeState()
{
}

void CTreeShakeState::Enter()
{
    CMonsterState::Enter();
}

void CTreeShakeState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CTreeShakeState::Ready_State()
{
    m_dwCurStateKey = CTreeMob::TREE_STATE::ATK_SHAKE;

    m_fMinStateTime = 1.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CTreeShakeState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    return 0;
}

void CTreeShakeState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CTreeShakeState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CTreeMob::TREE_STATE::IDLE);
}

void CTreeShakeState::Determine_NextState()
{
    m_pStateMachine->Change_State(CTreeMob::TREE_STATE::IDLE);
}

CTreeShakeState* CTreeShakeState::Create(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
{
    CTreeShakeState* pIdle = new CTreeShakeState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("CTreeShakeState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CTreeShakeState::Free()
{
}
