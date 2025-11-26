#include "pch.h"
#include "CCombatComponent.h"

CCombatComponent::CCombatComponent(CGameObject* pOwner, CCombatStats* pStats)
    : CEntityComponent(pOwner), m_pCombatStats(pStats)
    , m_bTryParrying(false)
{
  
}

CCombatComponent::CCombatComponent(const CCombatComponent& rhs)
    : CEntityComponent(rhs)
    , m_bTryParrying(rhs.m_bTryParrying)
{
    m_pCombatStats = rhs.m_pCombatStats ? static_cast<CCombatStats*>(rhs.m_pCombatStats->Clone()) : nullptr;
}

CCombatComponent::~CCombatComponent()
{
}

void CCombatComponent::Take_Damage(const DAMAGE_INFO& damage)
{
    // 패링
    //if (damage.bCanParry && m_bTryParrying)
    //{
    //    m_OnSuccessParrying();
    //    return;
    //}

    // 최종 데미지 : 총 공격량 - 방어량
    _float fResDamage = damage.fAmount - m_pCombatStats->Get_DefenseStat()->Get_CurValue();
    m_pCombatStats->Get_HealthStat()->Decrease(fResDamage);

    // 넉백
    // 데미지 적용 이후 죽지 않았다면 진행
    //if (damage.bShouldKnockback)
    //{
    //    m_OnShouldKnockback(damage.vDirKnockback);
    //}
}

CCombatComponent* CCombatComponent::Create(CGameObject*pOwner, CCombatStats* pStats)
{
    return new CCombatComponent(pOwner, pStats);
}

CComponent* CCombatComponent::Clone()
{
    return new CCombatComponent(*this);
}

void CCombatComponent::Free()
{
    m_pCombatStats->Release();
}
