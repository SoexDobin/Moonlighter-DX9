#include "CCollisionManager.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
    Free();
}

void CCollisionManager::Add_Collider(const wstring& wsLayerTag, CCollider* pColComonent)
{
    m_vecCollider.push_back(pColComonent);
    pColComonent->AddRef();
}

void CCollisionManager::Release_Collider()
{
    for_each(m_vecCollider.begin(), m_vecCollider.end(),
        [](CCollider* pCol) -> void {
            Safe_Release(pCol);
        });
    m_vecCollider.clear();
}

void CCollisionManager::Update_Collision()
{
    size_t iColSize = m_vecCollider.size();

    for (size_t i = 0; i < iColSize; ++i)
    {
        CCollider* pSrc = m_vecCollider[i];
        if (pSrc == nullptr || pSrc->Get_Owner() == nullptr) continue;

        for (size_t j = i + 1; j < iColSize; ++j)
        {
            CCollider* pDst = m_vecCollider[j];
            if (pDst == nullptr || pDst->Get_Owner() == nullptr) continue;

            pSrc->Check_Collision(pDst);

            if (pDst->Get_IsCol())
            {

            }
            else
            {

            }
            
        }
    }
}

_bool CCollisionManager::RectCollision()
{
    return true;
}

_bool CCollisionManager::SphereCollision(CSphereCollider* pSrc, CSphereCollider* pDst)
{

    return true;
}

void CCollisionManager::Free()
{
    Release_Collider();
}
