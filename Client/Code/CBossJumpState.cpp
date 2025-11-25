#include "pch.h"
#include "CBossJumpState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"

CBossJumpState::CBossJumpState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_fDeltaX(0.f), m_fDeltaY(0.f), m_fDeltaZ(0.f), m_fJumpDuration(0.f), m_fDiffX(0.f), m_fDiffZ(0.f)
    , m_bJumping(false), m_bJumpCompleted(false)
{

}

CBossJumpState::~CBossJumpState()
{
}

void CBossJumpState::Enter()
{
    CMonsterState::Enter();
}

void CBossJumpState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossJumpState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_JUMP;

    m_fMinStateTime = 0.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));

    return S_OK;
}

_uint CBossJumpState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    if (m_bJumping) 
        Jump_ToPlayer(fTimeDelta);

    return 0;
}

void CBossJumpState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransiti(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossJumpState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_JUMP);
}

void CBossJumpState::Check_EventFrame()
{
    CMonsterState::Check_EventFrame();

    // 점프 시작 
    if (m_dwCurFrame == 10)
        Ready_Jump();
}

void CBossJumpState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::ATK_JUMP);
}

void CBossJumpState::Jump_ToPlayer(const _float fTimeDelta)
{
    // TODO : 점프 모양의 함수 만들기 








}

void CBossJumpState::Ready_Jump()
{
    // 점프 도착 위치
    m_vDstPos = m_pPlayerTransformCom->Get_Pos();

    _vec3 vDiffToPlayer = m_pOwnerTransformCom->Get_Pos() - m_vDstPos;

    // delat X, Z 구하기
    m_fDeltaX = vDiffToPlayer.x / m_fJumpDuration;
    m_fDeltaZ = vDiffToPlayer.z / m_fJumpDuration;

    m_fOffsetY = -1.f * powf(m_pOwnerTransformCom->Get_Pos().x, 2);

    m_bJumping = true;
}

void CBossJumpState::Compute_DeltaY()
{
    // _float fNewY = 
}

CBossJumpState* CBossJumpState::Create(CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossJumpState* pState = new CBossJumpState(pOwner, pStateMachine);

    if (FAILED(pState->Ready_State()))
    {
        MSG_BOX("BossJumpState Create Failed!");
        return nullptr;
    }

    return pState;
}

void CBossJumpState::Free()
{

}
