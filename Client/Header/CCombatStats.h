#pragma once
#include "CStatComponent.h"

class CCombatStats : public CEntityComponent
{
public:
    CCombatStats(CGameObject* pOwner, CStatComponent* health, CStatComponent* attack, CStatComponent* defense, CStatComponent* speed);
    CCombatStats(const CCombatStats& rhs);
    virtual ~CCombatStats() {};

public:
    void Set_HealthStat(CStatComponent* stat) { m_pHealth = stat; }
    void Set_AttackStat(CStatComponent* stat) { m_pAttack = stat; }
    void Set_DefenseStat(CStatComponent* stat) { m_pDefense = stat; }
    void Set_SpeedStat(CStatComponent* stat) { m_pSpeed = stat; }

    CStatComponent* Get_HealthStat() { return m_pHealth; }
    CStatComponent* Get_AttackStat() { return m_pAttack; }
    CStatComponent* Get_DefenseStat() { return m_pDefense; }
    CStatComponent* Get_SpeedStat() { return m_pSpeed; }

private:
    CStatComponent* m_pHealth;
    CStatComponent* m_pAttack;
    CStatComponent* m_pDefense;
    CStatComponent* m_pSpeed;

public:
    static CCombatStats* Create(CGameObject* pOwner,
                                            CStatComponent* health = nullptr,
                                            CStatComponent* attack = nullptr,
                                            CStatComponent* defense = nullptr,
                                            CStatComponent* speed = nullptr)
    {
        return new CCombatStats(pOwner, health, attack, defense, speed);
    }


public:
    virtual CComponent* Clone() override;

private:
    virtual void Free() override;

};
