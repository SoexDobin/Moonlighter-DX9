#include "pch.h"
#include "CHitSphereBox.h"
#include "CRenderObject.h"
#include "CCollisionManager.h"

CHitSphereBox::CHitSphereBox(LPDIRECT3DDEVICE9 pGrahpicDev, CGameObject* pOwner)
    :CSphereCollider(pGrahpicDev)
{
    // owner 설정은 CHitbox::Create()에서 이미 실행
}

CHitSphereBox::CHitSphereBox(const CHitSphereBox& rhs)
    : CSphereCollider(rhs)
{
    m_pOwner = rhs.m_pOwner;

    CCollisionManager::GetInstance()->Add_Collider(this);
}

CHitSphereBox::~CHitSphereBox()
{
}

CHitSphereBox* CHitSphereBox::Create(LPDIRECT3DDEVICE9 pGrahpicDev, CGameObject* pOwner)
{
    CHitSphereBox* pHitBox = new CHitSphereBox(pGrahpicDev, pOwner);

    if (RENDER_OBJECT == pOwner->Get_Type())
    {
        // 충돌 시 transform 접근을 위해 오너 등록 필수
        pHitBox->Set_Owner(pOwner);
        CCollisionManager::GetInstance()->Add_Collider(pHitBox);

        return pHitBox;
    }

    return nullptr;
}

CComponent* CHitSphereBox::Clone()
{
    return nullptr;
}

void CHitSphereBox::Free()
{
}
