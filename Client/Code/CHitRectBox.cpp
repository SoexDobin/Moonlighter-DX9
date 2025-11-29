#include "pch.h"
#include "CHitRectBox.h"
#include "CRenderObject.h"
#include "CCollisionManager.h"

CHitRectBox::CHitRectBox(LPDIRECT3DDEVICE9 pGrahpicDev, CGameObject* pOwner)
    :CRectCollider(pGrahpicDev)
{
    // owner 설정은 CHitbox::Create()에서 이미 실행
}

CHitRectBox::CHitRectBox(const CHitRectBox& rhs)
    : CRectCollider(rhs)
{
    m_pOwner = rhs.m_pOwner;

    CCollisionManager::GetInstance()->Add_Collider(this);
}

CHitRectBox::~CHitRectBox()
{
}

CHitRectBox* CHitRectBox::Create(LPDIRECT3DDEVICE9 pGrahpicDev, CGameObject* pOwner)
{
    CHitRectBox* pHitBox = new CHitRectBox(pGrahpicDev, pOwner);

    if (RENDER_OBJECT == pOwner->Get_Type())
    {
        // 충돌 시 transform 접근을 위해 오너 등록 필수
        pHitBox->Set_Owner(pOwner);
        CCollisionManager::GetInstance()->Add_Collider(pHitBox);

        return pHitBox;
    }

    return nullptr;
}

CComponent* CHitRectBox::Clone()
{
    return nullptr;
}

void CHitRectBox::Free()
{
}
