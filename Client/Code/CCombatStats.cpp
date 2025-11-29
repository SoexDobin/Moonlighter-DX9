#include "pch.h"
#include "CCombatStats.h"

CCombatStats::CCombatStats(CGameObject* pOwner, CStatComponent* health, CStatComponent* attack, CStatComponent* defense, CStatComponent* speed)
    :CEntityComponent(pOwner),
    m_pHealth(health), m_pAttack(attack), m_pDefense(defense), m_pSpeed(speed)
{
}

CCombatStats::CCombatStats(const CCombatStats& rhs)
    : CEntityComponent(rhs.m_pOwner)
{
    m_pHealth = rhs.m_pHealth ? static_cast<CStatComponent*>(rhs.m_pHealth->Clone()) : nullptr;
    m_pAttack = rhs.m_pAttack ? static_cast<CStatComponent*>(rhs.m_pAttack->Clone()) : nullptr;
    m_pDefense = rhs.m_pDefense ? static_cast<CStatComponent*>(rhs.m_pDefense->Clone()) : nullptr;
    m_pSpeed = rhs.m_pSpeed ? static_cast<CStatComponent*>(rhs.m_pSpeed->Clone()) : nullptr;
}

CComponent* CCombatStats::Clone()
{
    return new CCombatStats(*this);
}

void CCombatStats::Free()
{
    m_pHealth->Release();
    m_pAttack->Release();;
    m_pDefense->Release();
    m_pSpeed->Release();
}
