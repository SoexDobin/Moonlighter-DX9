#include "pch.h"
#include "CBossRootState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"
#include "CBossGroundBrust.h"
#include "CLayerHelper.h"

CBossRootState::CBossRootState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState()
    , m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_iCurrentBrustCnt(0)
{

}

CBossRootState::CBossRootState(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(pGraphicDev)
    , m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_iCurrentBrustCnt(0)
{
}

CBossRootState::~CBossRootState()
{
}

void CBossRootState::Enter()
{
    CMonsterState::Enter();

    m_pOwnerTextureCom->Set_Texture(CBoss::BOSS_STATE::ATK_ROOT, 0);

    m_iCurrentComboCnt = 0;
    m_iMaxComboCnt = rand() % 2 + 2;    // 최소 2회 ~ 최대 3회

    m_bAttacking = false;

    m_iAttackFrameRange = (m_iAttackEndFrame - m_iAttackStartFrame) / m_iMaxComboCnt;

    { // =============== 디버깅 후 지우자 ===============
        printf("이번 타임 공격 횟수 : %d\n", m_iMaxComboCnt);
        printf("콤보 간 프레임 간격 : %d\n", m_iAttackFrameRange);
    }
}

void CBossRootState::Exit()
{
    CMonsterState::Exit();
    printf("상태 종료\n\n");
}

HRESULT CBossRootState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_ROOT;

    m_fMinStateTime = 0.f;
    m_vecComputedSpawnPos.resize(m_iMaxBrustCnt);
    m_fCurCreateLimitTime = 0.04f;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));
    m_pOwnerTextureCom = static_cast<CTexture*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Texture_Com"));

    return S_OK;
}

_uint CBossRootState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    if (m_bAttacking)
    {
        m_fCurComboElapsedTime += fTimeDelta;

        // 각 Brust를 시간차 생성
        if (m_fCurComboElapsedTime > m_fCurCreateLimitTime)
        {
            m_fCurComboElapsedTime = 0.f;

            // 루트 타입 랜덤 생성 
            CBossGroundBrust* pBrust = CBossGroundBrust::Create(m_pGraphicDev, m_pOwner);
            if (nullptr != pBrust)
            {
                _int iRand = rand() % 2;

                if (iRand)
                    pBrust->Set_Root1();
                else
                    pBrust->Set_Root2();

                // 위치 설정
                pBrust->Set_SpawnPos(m_vecComputedSpawnPos[m_iCurrentBrustCnt++]);

                CLayerHelper::GetInstance()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L"BossGroundBrust", pBrust);

                if (m_iCurrentBrustCnt >= m_iMaxBrustCnt)
                {
                    // 현재 콤보 종료
                    m_bAttacking = false;
                    m_iCurrentComboCnt++;
                    m_iCurrentBrustCnt = 0;
                    return 0;
                }
            }
        }
    }

    return 0;
}

void CBossRootState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

    // 공격 다 했으면 전환 true
    if (m_iCurrentComboCnt >= m_iMaxComboCnt && m_dwCurFrame >= 55)
    {
        m_bCanTransit = true;
    }

    if (m_bCanTransit)
        Determine_NextState();
}

void CBossRootState::Render_State()
{
    m_pOwner->Set_CurAnimKey(CBoss::BOSS_STATE::ATK_ROOT);

    Check_EventFrame();
}

void CBossRootState::Check_EventFrame()
{
    CMonsterState::Check_EventFrame();

    // 최대 공격 횟수를 일정한 프레임마다 수행 
    if (!m_bAttacking   // 현재 공격 중이 아니라면, 
        && m_dwCurFrame == m_iAttackStartFrame + m_iAttackFrameRange * m_iCurrentComboCnt   // 프레임 간격마다 실행
        && m_dwCurFrame <= m_iAttackEndFrame // 공격 가능한 프레임 범위 내에서 
        )
    {
        printf("현재 프레임 : %d, %d번째 공격 시작\n", m_dwCurFrame, m_iCurrentComboCnt);
        Compute_SpawnPos();
        m_bAttacking = true;
        m_fCurComboElapsedTime = 0.f;
    }
}

void CBossRootState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::IDLE);
}

void CBossRootState::Ready_CurrentCombo()
{
    m_iCurrentBrustCnt = 0;
    m_fCurComboElapsedTime = 0.f;
}

void CBossRootState::Compute_SpawnPos()
{
    // TODO : 맵 유틸리티에게서 맵 사이즈 받아와야 한다
    // TODO : 오브젝트 위치도 받아와야 한다
    _vec3 vMinBound{ -60.f, 0.f, -60.f };
    _vec3 vMaxBound{ 40.f, 0.f, 40.f };

    _int    iSpawnRandRange = 5;
    _int    iOffset = 1.5;
    _int    iFailCount = 0;     // 실패 횟수 확인하는 용도

    m_vDetectedPos = m_pPlayerTransformCom->Get_Pos();

    for (int i = 0; i < m_iMaxBrustCnt; )
    {
        _int iRandX = (rand() % iSpawnRandRange) - static_cast<int>(iSpawnRandRange * 0.5f);
        _int iRandZ = (rand() % iSpawnRandRange)-  static_cast<int>(iSpawnRandRange * 0.5f);

        _vec3 vSpawnPos = m_vDetectedPos;
        vSpawnPos.x += iRandX;
        vSpawnPos.z += iRandZ;

        // 생성할 위치가 맵 범위 내에 있는지 확인 
        if (vSpawnPos.x > vMinBound.x + iOffset
            && vSpawnPos.x < vMaxBound.x - iOffset
            && vSpawnPos.z > vMinBound.z + iOffset
            && vSpawnPos.z < vMaxBound.z - iOffset)
        {
            m_vecComputedSpawnPos[i++] = vSpawnPos;

            // 디버깅용
            iFailCount = 0;
        }
        else
        {
            if (iFailCount++ > 10)
            {
                MSG_BOX("It needs to change create logic!!");
            }
        }
    }
}

CBossRootState* CBossRootState::Create(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine)
{
    CBossRootState* pIdle = new CBossRootState(pGraphicDev, pOwner, pStateMachine);

    if (FAILED(pIdle->Ready_State()))
    {
        MSG_BOX("BossRootState Create Failed!");
        return nullptr;
    }

    return pIdle;
}

void CBossRootState::Free()
{

}
