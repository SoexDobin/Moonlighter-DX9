#include "pch.h"
#include "CBossRootState.h"
#include "CBoss.h"
#include "CBossStateMachine.h"
#include "CBossGroundBrust.h"
#include "CLayerHelper.h"

CBossRootState::CBossRootState(CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState()
    , m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_bCanCreateBrust(false), m_iCurrentBrustCnt(0)
{

}

CBossRootState::CBossRootState(LPDIRECT3DDEVICE9 pGraphicDev, CBoss* pOwner, CBossStateMachine* pStateMachine)
    : CMonsterState(pGraphicDev)
    , m_pOwner(pOwner), m_pStateMachine(pStateMachine)
    , m_bCanCreateBrust(false), m_iCurrentBrustCnt(0)
{
}

CBossRootState::~CBossRootState()
{
}

void CBossRootState::Enter()
{
    CMonsterState::Enter();

    m_bCanCreateBrust = false;
    m_iCurrentBrustCnt = 0;
}

void CBossRootState::Exit()
{
    CMonsterState::Exit();
}

HRESULT CBossRootState::Ready_State()
{
    m_dwCurStateKey = CBoss::BOSS_STATE::ATK_ROOT;

    m_fMinStateTime = 0.f;

    m_bCanCreateBrust = false;

    m_pOwnerTransformCom = static_cast<CTransform*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Transform_Com"));
    m_pOwnerTextureCom = static_cast<CTexture*>(m_pOwner->Get_Component(ID_DYNAMIC, L"Texture_Com"));

    return S_OK;
}

_uint CBossRootState::Update_State(const _float& fTimeDelta)
{
    CMonsterState::Detect_Player();

    if (m_bCanCreateBrust)
    {
        CBossGroundBrust* pBrust = CBossGroundBrust::Create(m_pGraphicDev, m_pOwner);
        if (nullptr != pBrust)
        {
            _int iRand = rand() % 2;

            if (iRand)
                pBrust->Set_Root1();
            else
                pBrust->Set_Root2();

            _vec3 vSpawnPos = m_vDetectedPos;
            vSpawnPos.z -= (m_iCurrentBrustCnt - 5);
            pBrust->Set_SpawnPos(vSpawnPos);

            m_iCurrentBrustCnt++;
        }

        CLayerHelper::GetInstance()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L"BossGroundBrust", pBrust);
    }


    return 0;
}

void CBossRootState::LateUpdate_State(const _float& fTimeDelta)
{
    CMonsterState::Check_ShouldTransit(fTimeDelta);

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

    if (m_dwCurFrame == 15)
    {
        m_vDetectedPos = m_pPlayerTransformCom->Get_Pos();
    }
    if (m_dwCurFrame == 18)
    {
        m_bCanCreateBrust = true;
    }
    if (m_dwCurFrame == 25)
    {
        m_bCanCreateBrust = false;
    }
    if (m_dwCurFrame == 55)
    {
        m_bCanTransit = true;
    }
}

void CBossRootState::Determine_NextState()
{
    m_pStateMachine->Change_State(CBoss::BOSS_STATE::IDLE);
}

void CBossRootState::Ready_Combat()
{

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
