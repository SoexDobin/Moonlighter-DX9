#include "CCollider.h"

CCollider::CCollider()
    : CComponent(), m_bIsCol(false)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), m_bIsCol(false)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
}

CCollider::CCollider(const CCollider& rhs)
    : CComponent(rhs), m_bIsCol(false)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
}

CCollider::~CCollider()
{
}

void CCollider::Add_OverlapMember(CCollider* pOverlap)
{
    m_usetOverlapCol.insert(pOverlap);
}

void CCollider::Release_OverlapMember(CCollider* pOverlap)
{
    m_usetOverlapCol.erase(pOverlap);
}

void CCollider::Frame_Release()
{
    
}

void CCollider::Free()
{
    Frame_Release();
    CComponent::Free();
}
