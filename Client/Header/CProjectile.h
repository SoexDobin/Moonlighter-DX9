#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CCollider;
}

class CProjectile : public CRenderObject // Buffer와 Transform 소유 
{
protected :
    explicit CProjectile(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CProjectile(const CProjectile& rhs);
    virtual ~CProjectile();

public :
    HRESULT     Ready_GameObject() override;
    _int        Update_GameObject(const _float fTimeDelta) override;
    void        LateUpdate_GameObject(const _float fTimeDelta) override;
    void        Render_GameObject() override;
    void        On_Collision(const Collision& tCollision) override;

public :
    void        Set_ProjectileInfo(const PROJECTILE& tInfo);
    void        Set_ShootDir(const _vec3& vDir);
    void        Set_Shooting() { m_bShooting = true; m_bReadyShoot = false; }

protected:
    virtual void        Ready_Shoot(const _float& fTimeDelta) {};
    virtual void        Shooting(const _float& fTimeDelta) {};
    virtual void        Post_Impact(const _float& fTimeDelta) {};

protected:
    HRESULT     Ready_Texture();

protected:
    CTexture* m_pDynamicTexCom;
    CCollider* m_pColCom;
    PROJECTILE m_tInfo;

    _vec3 m_vShootDir;
    _bool m_bReadyShoot, m_bShooting, m_bHasImpacted;

public:
    static CProjectile* Create(LPDIRECT3DDEVICE9 pGraphicDev, PROJECTILE tInfo);

protected:
    void                Free() override;

};

