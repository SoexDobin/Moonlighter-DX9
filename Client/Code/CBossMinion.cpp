#include "pch.h"
#include "CBossMinion.h"

#include "CHitRectBox.h"

CBossMinion::CBossMinion(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner)
    : CMonster(pGraphicDev)
    , m_pDynamicTexCom(nullptr),m_pOwner(pOwner)
{
}

CBossMinion::CBossMinion(const CBossMinion& rhs)
    : CMonster(rhs)
    , m_pDynamicTexCom(nullptr), m_pOwner(rhs.m_pOwner)
{
}

CBossMinion::~CBossMinion()
{
}

HRESULT CBossMinion::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    m_pTransformCom->Set_Scale({ 2.f, 2.f, 2.f });

    Ready_Combat();
    Ready_Texture();

    m_iObjectID = OBJECT_ID::MONSTER;

    m_bAscending = m_bFalling = m_bAnchored = false;

    return S_OK;
}

_int CBossMinion::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = CRenderObject::Update_GameObject(fTimeDelta);

    if (m_bAscending)
    {
        Ascending(fTimeDelta);
        return 0;
    }

    if (m_bFalling)
    {
        Falling(fTimeDelta);
        return 0;
    }

    if (m_bAnchored)
    {
        Anchored(fTimeDelta);
        return 0;
    }

    return iExit;
}

void CBossMinion::LateUpdate_GameObject(const _float fTimeDelta)
{
    CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CBossMinion::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pDynamicTexCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBossMinion::On_Collision(const Collision& tCollision)
{
    // 플레이어 공격 충돌 시 hp 깎기 
}

void CBossMinion::Ready_Combat()
{

}

void CBossMinion::Ready_Texture()
{
    m_pDynamicTexCom = Add_Component<CTexture>(ID_DYNAMIC, L"Texture_Com", TEXTURE);

    vector<wstring> vecTexture = { L"Boss_Cutting_Growing", L"Boss_Cutting_Idle", L"Boss_Cutting_Shot"};

    for (auto& texture : vecTexture)
        m_pDynamicTexCom->Ready_Texture(texture);
}

void CBossMinion::Ascending(const _float fTimeDelta)
{
    m_pTransformCom->Move_Pos(&vDir, fTimeDelta, m_fSpeed);

    if (m_pTransformCom->Get_Pos().y >= m_fMaxHeight)
    {
        _vec3 vNewPos = m_vAnchorPos;
        vNewPos.y = m_pTransformCom->Get_Pos().y;
        m_pTransformCom->Set_Pos(vNewPos);

        vDir *= -1; // 방향 전환
        m_bFalling = true;
        m_bAscending = false;
    }
}

void CBossMinion::Falling(const _float fTimeDelta)
{
    m_pTransformCom->Move_Pos(&vDir, fTimeDelta, m_fSpeed);

    if (m_pTransformCom->Get_Pos().y <= m_vAnchorPos.y + m_fEpsilon)
    {

        vDir *= 0.f;
        m_bFalling = true;
        m_bAscending = false;
    }
}

void CBossMinion::Anchored(const _float fTimeDelta)
{
}

CBossMinion* CBossMinion::Create(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner)
{
    CBossMinion* pObj = new CBossMinion(pGraphicDev, pOwner);

    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("CBossMinion Create Failed!!");
        return nullptr;
    }

    return pObj;
}

void CBossMinion::Free()
{
    CRenderObject::Free();
}
