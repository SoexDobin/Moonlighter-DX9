#pragma once
#include "CEntityComponent.h"
#include "CGameObject.h"
#include "CCombatStats.h"

using Callback = std::function<void()>;
using Callback_Vec3 = std::function<void(_vec3)>;

typedef struct tagDamageInfo
{
    CGameObject* pAttacker;

    _float fAmount;                  // 공격량
    _bool bCanParry;                // 패링 가능 여부
    _bool bShouldKnockback;   // 넉백 적용

    //FIXME : 넉백 방향 CollisionManager에서 받아올 수 있는지 확인 
    _vec3 vDirKnockback;         // 넉백 방향 
}DAMAGE_INFO;


class CCombatComponent : public CEntityComponent
{
private :
    CCombatComponent(CGameObject* pOwner, CCombatStats* pStats);
    CCombatComponent(const CCombatComponent& rhs);
    virtual ~CCombatComponent();

public :
    void    Set_TryParrying(_bool bParrying) { m_bTryParrying = bParrying; }
    void    Set_CurOwnerDir(const _vec3 vDir) { m_vCurOwnerDir = vDir; }
    void    Take_Damage(const DAMAGE_INFO& damage);

private :
    CCombatStats*    m_pCombatStats;

    _bool   m_bTryParrying;
    Callback m_OnSuccessParrying;

    _vec3   m_vCurOwnerDir;
    Callback_Vec3 m_OnShouldKnockback;


public :
    static CCombatComponent* Create(CGameObject* pOwner, CCombatStats* pStats);

    CComponent* Clone() override;

private:
    virtual void Free() override;
};

