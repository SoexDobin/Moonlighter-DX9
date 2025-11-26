#pragma once
#include "CCombatStats.h"
#include "CHitRectBox.h"
#include "CHitSphereBox.h"

using Callback = std::function<void()>;
using Callback_Vec3 = std::function<void(_vec3)>;

class CCombatComponent : public CEntityComponent
{
private :
    CCombatComponent(CGameObject* pOwner, CCombatStats* pStats);
    CCombatComponent(const CCombatComponent& rhs);
    virtual ~CCombatComponent();

public :
    void    Set_TryParrying(_bool bParrying) { m_bTryParrying = bParrying; }
    void    Set_CurOwnerDir(const _vec3 vDir) { m_vCurOwnerDir = vDir; }
    void    Take_Damage(CCollider* pHitbox, const DAMAGE_INFO& damage);

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

