#pragma once
#include "CSphereCollider.h"

class CHitSphereBox : public CSphereCollider
{
private:
    explicit CHitSphereBox(LPDIRECT3DDEVICE9 pGrahpicDev);
    explicit CHitSphereBox(const CHitSphereBox& rhs);
    virtual ~CHitSphereBox();

public:
    void    Set_Damage(const DAMAGE_INFO& tDamage) { m_tDamage = tDamage; }
    const DAMAGE_INFO& Get_Damage() const { return m_tDamage; }
private:
    DAMAGE_INFO   m_tDamage;

public:
    static CHitSphereBox* Create(LPDIRECT3DDEVICE9 pGrahpicDev);
    virtual CComponent* Clone() override;

private:
    virtual void Free() override;
};

