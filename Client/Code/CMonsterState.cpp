#pragma region INCLUDE
#include "pch.h"
#include "CMonsterState.h"
#include "CMonsterStateMachine.h"

#include "CManagement.h"

#include "CPlayer.h"

#include "CTransform.h"
#pragma endregion

CMonsterState::CMonsterState()
    : m_pMonsterStateMachine(nullptr),
    m_pPlayer(nullptr), m_pPlayerTransformCom(nullptr)
{

}

CMonsterState::CMonsterState(CStateMachine* pStateMachine)
    :  m_pMonsterStateMachine(static_cast<CMonsterStateMachine*>(pStateMachine)),
    m_pPlayer(nullptr), m_pPlayerTransformCom(nullptr)

{

}

CMonsterState::~CMonsterState()
{
}

void CMonsterState::Enter()
{
    Find_Player();

}

void CMonsterState::Exit()
{
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

    CComponent* pComponent = CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic_Layer", L"Player", L"Com_Transform");
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

}
