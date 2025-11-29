#pragma once
#include "CMonster.h"

class CHitRectBox;

class CBossMinion : public CMonster
{
private:
    explicit CBossMinion(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner);
    explicit CBossMinion(const CBossMinion& rhs);
    virtual ~CBossMinion();

public:
    HRESULT     Ready_GameObject() override;
    _int        Update_GameObject(const _float fTimeDelta) override;
    void        LateUpdate_GameObject(const _float fTimeDelta) override;
    void        Render_GameObject() override;

    void        On_Collision(const Collision& tCollision) override;

private :
    void Ready_EntityComponent() override {};
    void Configure_Component() override {};
public:
    void        Shoot() { m_bAscending = true; }
    void        Set_AnchorPos(const _vec3& vPos) { m_vAnchorPos = vPos; }

private:
    void        Ready_Combat();
    void        Ready_Texture();

    void        Ascending(const _float fTimeDelta);
    void        Falling(const _float fTimeDelta);
    void        Anchored(const _float fTimeDelta);

private:
    CTexture* m_pDynamicTexCom;     // 텍스처

private:
    CRenderObject* m_pOwner;
    DAMAGE_INFO m_tDamage;              // 미니언이 발사할 투사체의 데미지 정보

    _bool       m_bAscending, m_bFalling, m_bAnchored;
    _float      m_bLastShootTime, m_bLimitShootTime;

    _float      m_fSpeed;
    _float      m_fMaxHeight;
    _vec3 vDir = { 0.f, 1.f, 0.f },   m_vAnchorPos;
    const _float      m_fEpsilon = 0.1f;



public:
    static CBossMinion* Create(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner);

protected:
    void                Free() override;
};

