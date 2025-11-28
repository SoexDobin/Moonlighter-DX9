#include "pch.h"
#include "CBossProjectile.h"

CBossProjectile::CBossProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
    : CProjectile(pGraphicDev)
{
}

CBossProjectile::CBossProjectile(const CProjectile& rhs)
    : CProjectile(rhs)
{
}

CBossProjectile::~CBossProjectile()
{
}

HRESULT CBossProjectile::Ready_GameObject()
{
    CProjectile::Ready_GameObject();

    return S_OK;
}

_int CBossProjectile::Update_GameObject(const _float fTimeDelta)
{
    return _int();
}

void CBossProjectile::LateUpdate_GameObject(const _float fTimeDelta)
{
}

void CBossProjectile::Render_GameObject()
{
}

void CBossProjectile::On_Collision(const Collision& tCollision)
{
    m_bShooting = false;
    m_bHasImpacted = true;

}

void CBossProjectile::Ready_Shoot(const _float& fTimeDelta)
{
    m_bReadyShoot = true;
    m_bShooting = m_bHasImpacted = false;
}

void CBossProjectile::Shooting(const _float& fTimeDelta)
{
    m_pTransformCom->Move_Pos(&m_vShootDir, fTimeDelta, m_tInfo.fSpeed);
}

void CBossProjectile::Post_Impact(const _float& fTimeDelta)
{

}

CBossProjectile* CBossProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev, PROJECTILE tInfo)
{
    CBossProjectile* pProjectile = new CBossProjectile(pGraphicDev);

    pProjectile->Set_ProjectileInfo(tInfo);
    if (FAILED(pProjectile->Ready_GameObject()))
    {
        Safe_Release(pProjectile);
        MSG_BOX("Create Projectile Failed");
        return nullptr;
    }

    return pProjectile;
}


void CBossProjectile::Free()
{
    CRenderObject::Free();
}
