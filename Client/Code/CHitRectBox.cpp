#include "pch.h"
#include "CHitRectBox.h"
#include "CRenderObject.h"
#include "CCollisionManager.h"

CHitRectBox::CHitRectBox(LPDIRECT3DDEVICE9 pGrahpicDev)
    :CRectCollider(pGrahpicDev)
{
    ZeroMemory(&m_tDamage, sizeof(m_tDamage));
}

CHitRectBox::CHitRectBox(const CHitRectBox& rhs)
    : CRectCollider(rhs), m_tDamage(rhs.m_tDamage)
{
}

CHitRectBox::~CHitRectBox()
{
}

CHitRectBox* CHitRectBox::Create(LPDIRECT3DDEVICE9 pGrahpicDev)
{
    CHitRectBox* pInstance = new CHitRectBox(pGrahpicDev);

    if (FAILED(pInstance->Ready_RectCollider()))
    {
        MSG_BOX("CHitRectBox Created Failed");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CHitRectBox::Clone()
{
    CHitRectBox* pClone = new CHitRectBox(*this);

    CCollisionManager::GetInstance()->Add_Collider(pClone);

    return pClone;
}

void CHitRectBox::Free()
{
    CRectCollider::Free();
}
