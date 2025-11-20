#include "CCollider.h"

CCollider::CCollider()
    : CComponent()
{
}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
}

CCollider::CCollider(const CCollider& rhs)
    : CComponent(rhs)
{
}

CCollider::~CCollider()
{
}

void CCollider::Free()
{
    CComponent::Free();
}
