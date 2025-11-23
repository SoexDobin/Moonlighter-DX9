#include "CCollisionManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CSphereCollider.h"
#include "CRectCollider.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
    Free();
}

void CCollisionManager::Add_Collider(CCollider* pColComonent)
{
    m_vecCollider.push_back(pColComonent);
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

            CGameObject* pSrcOwner = pSrc->Get_Owner();
            CGameObject* pDstOwner = pDst->Get_Owner();

            _bool bNowCol = pSrc->Check_Collision(pDst);

            if (bNowCol)
            {
                if (pSrc->Is_Overlapped(pDst))
                {
                    pSrc->Set_ColState(STAY_COL);
                    pSrc->Set_Collision({ pDstOwner, STAY_COL });
                }
                else
                {
                    pSrc->Add_OverlapMember(pDst);
                    pSrc->Set_ColState(ENTER_COL);
                    pSrc->Set_Collision({ pDstOwner, ENTER_COL });
                }

                if (pDst->Is_Overlapped(pSrc))
                {
                    pDst->Set_ColState(STAY_COL);
                    pDst->Set_Collision({ pSrcOwner, STAY_COL });
                }
                else
                {
                    pDst->Add_OverlapMember(pSrc);
                    pDst->Set_ColState(ENTER_COL);
                    pDst->Set_Collision({ pSrcOwner, ENTER_COL });
                }

                pSrcOwner->On_Collision(pDst->Get_Collision());
                pDstOwner->On_Collision(pSrc->Get_Collision());
            }
            else
            {
                if (pSrc->Is_Overlapped(pDst))
                {
                    pSrc->Set_ColState(EXIT_COL);
                    pSrc->Set_Collision({ pDstOwner, EXIT_COL });
                    pSrcOwner->On_Collision(pSrc->Get_Collision());
                    pSrc->Release_OverlapMember(pDst);
                }
                if (pDst->Is_Overlapped(pSrc))
                {
                    pDst->Set_ColState(EXIT_COL);
                    pDst->Set_Collision({ pSrcOwner, EXIT_COL });
                    pDstOwner->On_Collision(pDst->Get_Collision());
                    pDst->Release_OverlapMember(pSrc);
                }
            }
            
        }
    }
}

#ifdef _DEBUG
void CCollisionManager::Render_Collision()
{
    for (auto& pCol : m_vecCollider)
    {
        pCol->Render_DebugCollider();
    }
}
#endif

_bool CCollisionManager::RectCollision(CRectCollider* pSrc, CRectCollider* pDst)
{

    return true;
}

_bool CCollisionManager::SphereCollision(CSphereCollider* pSrc, CSphereCollider* pDst)
{
    _vec3 vSrcPos = pSrc->Get_Transform()->Get_Pos();
    _vec3 vDstPos = pDst->Get_Transform()->Get_Pos();
    _vec3 vDistance = vDstPos - vSrcPos;
    _float fDistance = D3DXVec3LengthSq(&vDistance);
    
    _float m_fRadiusSum = (pSrc->Get_Radius() * pSrc->Get_Scale() )
                        + (pDst->Get_Radius() * pSrc->Get_Scale() ) * 0.5f;

    return m_fRadiusSum >= fDistance;
}

void CCollisionManager::Free()
{
    for_each(m_vecCollider.begin(), m_vecCollider.end(),
        [](CCollider* pCol) -> void {
            Safe_Release(pCol);
        });
    m_vecCollider.clear();
}
