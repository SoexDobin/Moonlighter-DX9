#include "pch.h"
#include "CHitSphereBox.h"
#include "CRenderObject.h"
#include "CCollisionManager.h"

CHitSphereBox::CHitSphereBox(LPDIRECT3DDEVICE9 pGrahpicDev)
    :CSphereCollider(pGrahpicDev)
{
    ZeroMemory(&m_tDamage, sizeof(m_tDamage));
}

CHitSphereBox::CHitSphereBox(const CHitSphereBox& rhs)
    : CSphereCollider(rhs), m_tDamage(rhs.m_tDamage)
{
}

CHitSphereBox::~CHitSphereBox()
{
}

CHitSphereBox* CHitSphereBox::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
    CHitSphereBox* pInstance = new CHitSphereBox(pGrahpicDev);

    if (FAILED(pInstance->Ready_SphereCollider()))
    {
        MSG_BOX("CHitSphereBox Created Failed");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CHitSphereBox::Clone()
{
    CHitSphereBox* pClone = new CHitSphereBox(*this);

    CCollisionManager::GetInstance()->Add_Collider(pClone);

    return pClone;
}

void CHitSphereBox::Free()
{
    CSphereCollider::Free();
}
