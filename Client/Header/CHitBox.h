#pragma once
#include "CCollider.h"
class CHitBox :  public CComponent
{
private :
    explicit CHitBox(LPDIRECT3DDEVICE9 pGrahpicDev, COL_TYPE eColType, CGameObject* pOwner);
    explicit CHitBox(const CHitBox& rhs);
    virtual ~CHitBox();


public :
    static CHitBox* Create(LPDIRECT3DDEVICE9 pGrahpicDev,
                                            COL_TYPE eColType,
                                            CGameObject* pOwner);
    
private :
    CGameObject*    m_pOwner;
    CCollider*           m_pCollider;
    COL_TYPE          m_eColType;
};

