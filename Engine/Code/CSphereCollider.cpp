#include "CSphereCollider.h"
#include "CCollisionManager.h"

CSphereCollider::CSphereCollider()
{
}

CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCollider(pGraphicDev)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
    : CCollider(rhs)
{
}

CSphereCollider::~CSphereCollider()
{
}

HRESULT CSphereCollider::Ready_SphereCollider()
{
    return S_OK;
}

_bool CSphereCollider::Check_Collision(CCollider* pCol)
{
    switch (pCol->Get_ColType())
    {
    case SPHERE_COL:
        return CCollisionManager::GetInstance()->SphereCollision(this, static_cast<CSphereCollider*>(pCol));
    }
}

#ifdef _DEBUG
void CSphereCollider::Render_DebugCollider()
{
    if (m_pTrans == nullptr) return;


}
#endif 

CSphereCollider* CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);

    if (FAILED(pInstance->Ready_SphereCollider()))
    {
        MSG_BOX("CCollier Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CComponent* CSphereCollider::Clone()
{
    return new CSphereCollider(*this);
}

void CSphereCollider::Free()
{
    CCollider::Free();
}
