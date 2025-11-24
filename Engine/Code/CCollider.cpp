#include "CCollider.h"

CCollider::CCollider()
    : CComponent(), m_bIsCol(false), m_eState(COL_STATE_END)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
    ZeroMemory(&m_vOffset, sizeof(_vec3));
}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), m_bIsCol(false), m_eState(COL_STATE_END)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
    ZeroMemory(&m_vOffset, sizeof(_vec3));
}

CCollider::CCollider(const CCollider& rhs)
    : CComponent(rhs), m_bIsCol(false), m_eState(COL_STATE_END),
    m_vOffset(rhs.m_vOffset)
{
    ZeroMemory(&m_tCollision, sizeof(Collision));
}

CCollider::~CCollider()
{
}

_bool CCollider::Is_Overlapped(CCollider* pOverlap)
{
     return (m_usetOverlapCol.find(pOverlap) != m_usetOverlapCol.end());
}

void CCollider::Add_OverlapMember(CCollider* pOverlap)
{
    m_usetOverlapCol.insert(pOverlap);
}

void CCollider::Release_OverlapMember(CCollider* pOverlap)
{
    m_usetOverlapCol.erase(pOverlap);
}

void CCollider::Free()
{
    CComponent::Free();
}
