#include "CRectCollider.h"
#include "CCollisionManager.h"

#ifdef _DEBUG
#include "CTransform.h"
#endif

CRectCollider::CRectCollider()
    : CCollider()
{
}

CRectCollider::CRectCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCollider(pGraphicDev), m_vDimension( { 1.f, 1.f, 1.f } )
{
}

CRectCollider::CRectCollider(const CRectCollider& rhs)
    : CCollider(rhs), m_vDimension(rhs.m_vDimension)
{
}

CRectCollider::~CRectCollider()
{
}

HRESULT CRectCollider::Ready_RectCollider()
{
    return S_OK;
}

_int CRectCollider::Update_Component(const _float fTimeDelta)
{
    _int iExit = CCollider::Update_Component(fTimeDelta);

    return iExit;
}

void CRectCollider::LateUpdate_Component()
{
    CCollider::LateUpdate_Component();
}

_bool CRectCollider::Check_Collision(CCollider* pCol)
{
    switch (pCol->Get_ColType())
    {
    case SPHERE_COL:
        return true;
    case RECT_COL:
        return CCollisionManager::RectCollision(this, dynamic_cast<CRectCollider*>(pCol));
    default:
        return false;
    }
}

#ifdef _DEBUG
void CRectCollider::Render_DebugCollider()
{

}
#endif

CComponent* CRectCollider::Clone()
{
    CCollider* pClone = new CRectCollider(*this);
    //CCollisionManager::GetInstance()->Add_Collider(pClone);

    return pClone;
}

void CRectCollider::Free()
{
    CCollider::Free();
}
