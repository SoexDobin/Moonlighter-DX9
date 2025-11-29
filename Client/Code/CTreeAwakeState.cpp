#include "pch.h"
#include "CTreeAwakeState.h"
#include "CTreeMob.h"
#include "CTreeStateMachine.h"
#include "CTexture.h"

CTreeAwakeState::CTreeAwakeState(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{

}

CTreeAwakeState::~CTreeAwakeState()
{
}

void CTreeAwakeState::Enter()
{
    m_bStarted = true;
    m_bExited = false;
    m_bCanTransit = false;

    m_fCurStateElapsedTime = 0.f;
    m_bMinStateTimePassed = false;
}

void CTreeAwakeState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CTreeAwakeState::Ready_State()
{
    m_dwCurStateKey = CTreeMob::TREE_STATE::AWAKE;

    m_fMinStateTime = 0.5f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));
    m_pOwnerTextureCom = static_cast<CTexture*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Texture_Com"));

    return S_OK;
}

_uint CTreeAwakeState::Update_State(const _float& fTimeDelta)
{
    Check_EventFrame();

    return 0;
}

void CTreeAwakeState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CTreeAwakeState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CTreeMob::TREE_STATE::AWAKE);

    Check_EventFrame();
}

void CTreeAwakeState::Check_EventFrame()
{
    CMonsterState::Check_EventFrame();

    if (m_dwCurFrame == 7)
    {
        m_bCanTransit = true;
    }
}

void CTreeAwakeState::Determine_NextState()
{
    m_pStateMachine->Change_State(CTreeMob::TREE_STATE::IDLE);
}

CTreeAwakeState* CTreeAwakeState::Create(CTreeMob* pOwner, CTreeStateMachine* pStateMachine)
{
    CTreeAwakeState* pIdle = new CTreeAwakeState(pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("TreeAwakeState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CTreeAwakeState::Free()
{
}
