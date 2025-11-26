#pragma once
#include "CRectCollider.h"

class CHitRectBox :  public CRectCollider
{
private :
    explicit CHitRectBox(LPDIRECT3DDEVICE9 pGrahpicDev, CGameObject* pOwner);
    explicit CHitRectBox(const CHitRectBox& rhs);
    virtual ~CHitRectBox();

public :
    void    Set_Damage(const DAMAGE_INFO& tDamage) { m_tDamage = tDamage; }
    const DAMAGE_INFO& Get_Damage() const { return m_tDamage; }
private:
    DAMAGE_INFO   m_tDamage;

public :
    static CHitRectBox* Create(LPDIRECT3DDEVICE9 pGrahpicDev, CGameObject* pOwner);
    virtual CComponent* Clone() override;

private:
    virtual void Free() override;
};

