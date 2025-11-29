#include "pch.h"
#include "CProjectile.h"
#include "CHitRectBox.h"
#include  "CHitSphereBox.h"

CProjectile::CProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev)
    , m_pDynamicTexCom(nullptr), m_pColCom(nullptr)
{
    ZeroMemory(&m_tInfo, sizeof(m_tInfo));
}

CProjectile::CProjectile(const CProjectile& rhs)
    : CRenderObject(rhs)
{
}

CProjectile::~CProjectile()
{
}

HRESULT CProjectile::Ready_GameObject()
{
    CRenderObject::Ready_GameObject();

    if (FAILED(Ready_Texture())) // Texture 준비 
        return E_FAIL;

    if (FAILED(Ready_ProjectileInfo())) // 투사체 준비
        return E_FAIL;

    return S_OK;
}

_int CProjectile::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bReadyShoot)
    {
        // 발사 전 
        Ready_Shoot(fTimeDelta);
    }
    else if (m_bShooting)
    {
        // 발사 
        Shooting(fTimeDelta);
    }
    else if (m_bHasImpacted)
    {
        // 충돌 후
        Post_Impact(fTimeDelta);
    }

    return iExit;
}

void CProjectile::LateUpdate_GameObject(const _float fTimeDelta)
{
    CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CProjectile::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pDynamicTexCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CProjectile::On_Collision(const Collision& tCollision)
{

}

void CProjectile::Set_ProjectileInfo(const PROJECTILE& tInfo)
{
    m_tInfo = tInfo;
}

HRESULT CProjectile::Ready_Texture()
{
    if (m_pDynamicTexCom = Add_Component<CTexture>(ID_DYNAMIC, L"Texture_Com", TEXTURE))
    {
        for (auto& texture : m_tInfo.wsVecTexture)
            m_pDynamicTexCom->Ready_Texture(texture);

        m_pDynamicTexCom->Set_Speed(m_tInfo.fSpeed);

        return S_OK;
    }

    return E_FAIL;
}

HRESULT CProjectile::Ready_ProjectileInfo()
{
    // 히트박스 모양  결정 및 바로 데미지 정보 삽입 
    if (Engine::RECT_COL == m_tInfo.eColType)
    {
        m_pColCom = Add_Component<CHitRectBox>(ID_DYNAMIC, L"HitRectBox_Com", L"Hit_RectBox");
        static_cast<CHitRectBox*>(m_pColCom)->Set_Damage(m_tInfo.tDamageInfo);
        static_cast<CHitRectBox*>(m_pColCom)->Set_Scale(m_tInfo.fScale);

    }
    else if (Engine::SPHERE_COL == m_tInfo.eColType)
    {
        m_pColCom = Add_Component<CHitRectBox>(ID_DYNAMIC, L"HitSphereBox_Com", L"Hit_SpheretBox");
        static_cast<CHitSphereBox*>(m_pColCom)->Set_Damage(m_tInfo.tDamageInfo);
        static_cast<CHitSphereBox*>(m_pColCom)->Set_Scale(m_tInfo.fScale);
    }

    return S_OK;
}

CProjectile* CProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev, PROJECTILE tInfo)
{
    CProjectile* pProjectile = new CProjectile(pGraphicDev);

    // NOTE : PROJECTILE_INFO를 먼저 설정해야 Ready_GameObject()에서 이에 따른 변수 설정 진행
    pProjectile->Set_ProjectileInfo(tInfo);

    if (FAILED(pProjectile->Ready_GameObject()))
    {
        Safe_Release(pProjectile);
        MSG_BOX("Create Projectile Failed");
        return nullptr;
    }

    return pProjectile;
}

void CProjectile::Free()
{
    CRenderObject::Free();
}
