#include "pch.h"
#include "CBossJumpState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"
#include "EasingFunctions.h"
#include "CCameraManager.h"

CBossJumpState::CBossJumpState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(),
    m_pOwner(pOwner), m_pStateMachine(pStateMachine), m_pHitBox(nullptr)
    , m_fJumpDuration(0.f), m_fElapsed(0.f)
    , m_bReadyJump(false), m_bJumpingUp(false), m_bJumped(false)
{
    ZeroMemory(&m_tDamage, sizeof(m_tDamage));
}

CBossJumpState::~CBossJumpState()
{
}

void CBossJumpState::Enter()
{
    CMonsterState::Enter();

    m_pOwnerTextureCom->Set_Texture(CBoss::BOSS_STATE::ATK_JUMP, 0);

    m_bReadyJump =  m_bJumpingUp = m_bJumped = false;
}

void CBossJumpState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossJumpState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_JUMP;

    Ready_Combat();

    m_fMinStateTime = m_fElapsed =0.f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));
    m_pOwnerTextureCom = static_cast<CTexture*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Texture_Com"));

    m_fJumpDuration = 1.5f;

    return S_OK;
}

_uint CBossJumpState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    if (m_bReadyJump)
    {
        Ready_Jump();
        return 0;
    }
    if (m_bJumpingUp)
    {
        Jump_ToPlayer(fTimeDelta);
        return 0;
    }

    return 0;
}

void CBossJumpState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossJumpState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_JUMP);

    Check_EventFrame();
}

void CBossJumpState::Check_EventFrame()
{
    CMonsterState::Check_EventFrame();

    // 점프 시작 
    if (m_dwCurFrame == 10)
    {
        // 점프 프레임 총 10장
        // 프레임 수에 따라 애니메이션 속도 조절 필요하다 
        Ready_Jump();
        return;
    }

    if (m_dwCurFrame == 19) // 땅에 착지 
    {
        CCameraManager::GetInstance()->Start_Shake(1.f, 0.15f);
        // 히트박스 켜기
        m_pHitBox->On_Enable();
        return;
    }

    if (m_dwCurFrame == 24)
    {
        // 히트박스 끄기
        m_pHitBox->On_Disable();
        return;
    }


    if (m_dwCurFrame == 29) // 애니메이션 전부 재생 
    {
        m_bCanTransit = true;
        return;
    }
}

void CBossJumpState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::IDLE);
}

void CBossJumpState::Ready_Combat()
{
   //  점프 시에만 활성화 될 히트박스 
    m_pHitBox = m_pOwner->Add_Component<CHitRectBox>(ID_DYNAMIC, L"HitRectBox_Com", L"Hit_RectBox");

    m_tDamage.bCanParry = m_tDamage.bShouldKnockback = false;
    m_tDamage.vDirKnockback = { 0.f, 0.f, 0.f };
    m_tDamage.dwHitTargetFlag = OBJECT_ID::PLAYER;
    m_tDamage.eApplyTiming = COL_STATE::ENTER_COL;
    m_tDamage.fAmount = 15.f;
    m_tDamage.pAttacker = m_pOwner;

    m_pHitBox->Set_Damage(m_tDamage);
    m_pHitBox->Set_Dimension({ 12.f, 5.f, 10.f, });
    m_pHitBox->Set_Offset({ 0.f, -1.7f, 0.f });
    m_pHitBox->On_Disable();
}

void CBossJumpState::Jump_ToPlayer(const _float fTimeDelta)
{
    m_fElapsed += fTimeDelta;

    _float t = m_fElapsed / m_fJumpDuration;

    // 상승 -> 하강
    if (t >= 0.5f)
    {
        m_vDstPos.y = m_vOriginPos.y;
    }

    t = Ease::InOutQuad(t);

    if (t >= 1.f)
        t = 1.f;

    _vec3 vNewPos;
    D3DXVec3Lerp(&vNewPos, &m_vOriginPos, &m_vDstPos, t);
    m_pOwnerTransformCom->Set_Pos(vNewPos);

    if (t >= 1.f)
    {
        m_bJumped = true;
    }
}

void CBossJumpState::Ready_Jump()
{
    // 점프 도착 위치
    m_vDstPos = m_pPlayerTransformCom->Get_Pos();
    m_vDstPos.y += 18.f;

    m_vOriginPos = m_pOwnerTransformCom->Get_Pos();
    _vec3 vDiffToPlayer = m_pOwnerTransformCom->Get_Pos() - m_vDstPos;

    m_fElapsed = 0.f;

    m_bReadyJump  = false;
    m_bJumpingUp = true;
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
