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
    m_umCollider[wsLayerTag].push_back(pColComonent);
    pColComonent->AddRef();
}

void CCollisionManager::Release_Collider()
{
    for_each(m_umCollider.begin(), m_umCollider.end(),
        [](pair<const wstring, vector<CCollider*>>& pair) -> void {
            for_each(pair.second.begin(), pair.second.end(),
                [](CCollider* pCol) -> void {
                    Safe_Release(pCol);
                });
            pair.second.clear();
        });
    m_umCollider.clear();
}

void CCollisionManager::Update_Collision(const wstring& wsLayerTag)
{
    for (auto& pSrc : m_umCollider[wsLayerTag])
    {
        for (auto& pDst : m_umCollider[wsLayerTag])
        {
            
        }
    }
}

void CCollisionManager::Free()
{
    Release_Collider();
}
