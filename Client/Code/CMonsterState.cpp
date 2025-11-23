#pragma region INCLUDE
#include "pch.h"
#include "CMonsterState.h"
#include "CManagement.h"
#include "CPlayer.h"
#include "CTransform.h"
#include "CTexture.h"
#pragma endregion

CMonsterState::CMonsterState()
    : m_pPlayer(nullptr), m_pPlayerTransformCom(nullptr), m_pOwnerTransformCom(nullptr), m_pOwnerTextureCom(nullptr),
    m_fMinStateTime(0.f), m_fCurStateElapsedTime(0.f), m_bMinStateTimePassed(false), m_fDistToPlayer(0.f)
{
}

CMonsterState::~CMonsterState()
{
}

void CMonsterState::Enter()
{
    Find_Player();

    m_bStarted = true;
    m_bExited = false;
    m_bCanTransit = false;

    m_fCurStateElapsedTime = 0.f;
    m_bMinStateTimePassed = false;
}

void CMonsterState::Exit()
{
    m_bStarted = false;

    m_bExited = true;
}

void CMonsterState::Find_Player()
{
    if (nullptr != m_pPlayer)
        return;

    CGameObject* pObject = CManagement::GetInstance()->Get_Object(L"GameLogic_Layer", L"Player");

    if (nullptr == dynamic_cast<CPlayer*>(pObject))
    {
        MSG_BOX("MonsterState can't find player!");
        return;
    }

    m_pPlayer = static_cast<CPlayer*>(pObject);

    CComponent* pComponent = CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic_Layer", L"Player", L"Transform_Com");
    if (nullptr == dynamic_cast<CTransform*>(pComponent))
    {
        MSG_BOX("MonsterState can't find player's transform!");
        return;
    }
    m_pPlayerTransformCom = static_cast<CTransform*>(pComponent);
}

void CMonsterState::Detect_Player()
{
    if (nullptr == m_pOwnerTransformCom || nullptr == m_pPlayerTransformCom)
    {
        MSG_BOX("Null Reference Exception : CMonsterState::Detect_Player()");
        return;
    }

    _vec3 vDiff = m_pPlayerTransformCom->Get_Pos() - m_pOwnerTransformCom->Get_Pos();

    m_fDistToPlayer = D3DXVec3Length(&vDiff);
    D3DXVec3Normalize(&m_vDirToPlayer, &vDiff);
}

void CMonsterState::Check_ShouldTransiti(const _float& fTimeDelta)
{
    m_fCurStateElapsedTime += fTimeDelta;

    if (m_fCurStateElapsedTime >= m_fMinStateTime)
    {
        m_bMinStateTimePassed = true;
    }

}

void CMonsterState::Check_EventFrame()
{
    m_dwCurFrame = m_pOwnerTextureCom->Get_CurFrame();
}
