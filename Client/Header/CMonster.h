#pragma once
#include "CRenderObject.h"
#include "CCombatComponent.h"

namespace Engine
{
    class CCollider;
}

/// <summary>
/// Abstract class : Monster base
/// </summary>
class CMonster : public CRenderObject
{
public :
    explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev) :
        CRenderObject(pGraphicDev),
        m_pCombatStats(nullptr), m_pCombatComponent(nullptr), m_pColCom(nullptr){};
    explicit CMonster(const CMonster& rhs) : CRenderObject(rhs) {}
    virtual ~CMonster() {};

public:
    CStatComponent* Get_HealthStat() { return m_pCombatStats ? m_pCombatStats->Get_HealthStat() : nullptr; }
    CStatComponent* Get_AttackStat() { return m_pCombatStats ? m_pCombatStats->Get_AttackStat() : nullptr; }
    CStatComponent* Get_DefenseStat() { return m_pCombatStats ? m_pCombatStats->Get_DefenseStat() : nullptr; }
    CStatComponent* Get_SpeedStat() { return m_pCombatStats ? m_pCombatStats->Get_SpeedStat() : nullptr; }

public:
    CCombatStats* Get_CombatStats() { return m_pCombatStats; }

protected:
    virtual void    Ready_EntityComponent() PURE;
    virtual void    Configure_Component() PURE;

protected:
    CCombatStats* m_pCombatStats;
    CCombatComponent* m_pCombatComponent;

    CCollider* m_pColCom;   // 몬스터 자체의 콜라이더 (히트박스 아님)
};

