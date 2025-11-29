#include "pch.h"
#include "CBossAwakeState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"
#include "CTexture.h"

CBossAwakeState::CBossAwakeState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
{
}

CBossAwakeState::~CBossAwakeState()
{
}

void CBossAwakeState::Enter()
{
    // Monster::Enter()
    {
        m_bStarted = true;
        m_bExited = false;
        m_bCanTransit = false;

        m_fCurStateElapsedTime = 0.f;
        m_bMinStateTimePassed = false;
    }

    m_pOwnerTransformCom->Set_Scale({ 18.f, 18.f, 18.f });
    m_pOwnerTextureCom->Set_Loop(false);
}

void CBossAwakeState::Exit()
{
    CMonsterState::Exit();

    m_pOwnerTransformCom->Set_Scale({ 15.f, 15.f, 1.f });
    m_pOwnerTextureCom->Set_Loop(true);
}

HRESULT CBossAwakeState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::AWAKE;

    m_fMinStateTime = 0.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));
    m_pOwnerTextureCom = static_cast<CTexture*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Texture_Com"));

    return S_OK;
}

_uint CBossAwakeState::Update_State(const _float& fTimeDelta)
{
    Check_EventFrame();

    return 0;
}

void CBossAwakeState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossAwakeState::Render_State()
{
    // WALK와 IDLE은 같은 애니메이션 사용 
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::AWAKE);

}

void CBossAwakeState::Check_EventFrame()
{
    CMonsterState::Check_EventFrame();

    if (m_dwCurFrame == 128)
    {
        m_bCanTransit = true;
    }
}

void CBossAwakeState::Determine_NextState()
{
    if (m_bCanTransit)
    {
        // 원래 로직 : IDLE로 전환해야 한다
        m_pStateMachine->Change_State(CBoss::BOSS_STATE::IDLE);
    }
}

CBossAwakeState* CBossAwakeState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossAwakeState* pState = new CBossAwakeState(pOwner, pStateMachine);

    if (FAILED(pState->Ready_State()))
    {
        MSG_BOX("BossAwakeState Create Failed!");
        return nullptr;
    }

    return pState;
}

void CBossAwakeState::Free()
{

}
