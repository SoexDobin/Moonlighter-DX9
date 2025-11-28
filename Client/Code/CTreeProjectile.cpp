#include "pch.h"
#include "CTreeProjectile.h"

CTreeProjectile::CTreeProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
    : CProjectile(pGraphicDev)
{
}

CTreeProjectile::CTreeProjectile(const CProjectile& rhs)
    : CProjectile(rhs)
{
}

CTreeProjectile::~CTreeProjectile()
{
}

HRESULT CTreeProjectile::Ready_GameObject()
{
    CProjectile::Ready_GameObject();

    return S_OK;
}

_int CTreeProjectile::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = CProjectile::Update_GameObject(fTimeDelta);

    return iExit;
}

void CTreeProjectile::LateUpdate_GameObject(const _float fTimeDelta)
{
    CProjectile::LateUpdate_GameObject(fTimeDelta);
}

void CTreeProjectile::Render_GameObject()
{
    CProjectile::Render_GameObject();
}

void CTreeProjectile::On_Collision(const Collision& tCollision)
{
    m_bShooting = false;
    m_bHasImpacted = true;
}

void CTreeProjectile::Ready_Shoot(const _float& fTimeDelta)
{
    m_bReadyShoot = true;
    m_bShooting = m_bHasImpacted = false;
}


void CTreeProjectile::Shooting(const _float& fTimeDelta)
{
    m_pTransformCom->Move_Pos(&m_vShootDir, fTimeDelta, m_tInfo.fSpeed);

}

void CTreeProjectile::Post_Impact(const _float& fTimeDelta)
{
}

CTreeProjectile* CTreeProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev, PROJECTILE tInfo)
{
    CTreeProjectile* pProjectile = new CTreeProjectile(pGraphicDev);

    pProjectile->Set_ProjectileInfo(tInfo);
    if (FAILED(pProjectile->Ready_GameObject()))
    {
        Safe_Release(pProjectile);
        MSG_BOX("Create Projectile Failed");
        return nullptr;
    }

    return pProjectile;
}


void CTreeProjectile::Free()
{
    CRenderObject::Free();
}
