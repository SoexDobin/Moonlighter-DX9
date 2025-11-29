#include "pch.h"
#include "CBossGroundBrust.h"
#include "CHitRectBox.h"

CBossGroundBrust::CBossGroundBrust(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner)
    : CRenderObject(pGraphicDev)
    , m_pDynamicTexCom(nullptr),  m_pColCom(nullptr)
    , m_eType(ROOT_END), m_dwEndFrame(0), m_pOwner(pOwner)
{
}

CBossGroundBrust::CBossGroundBrust(const CBossGroundBrust& rhs)
    : CRenderObject(rhs)
    , m_pDynamicTexCom(nullptr), m_pColCom(nullptr)
    , m_eType(ROOT_END), m_dwEndFrame(0), m_pOwner(rhs.m_pOwner)
{
}

CBossGroundBrust::~CBossGroundBrust()
{
}

HRESULT CBossGroundBrust::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    m_pTransformCom->Set_Scale({ 4.f, 4.f, 4.f });

    Ready_Combat();
    Ready_Texture();

    return S_OK;
}

_int CBossGroundBrust::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = CRenderObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CBossGroundBrust::LateUpdate_GameObject(const _float fTimeDelta)
{
    CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CBossGroundBrust::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pDynamicTexCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    Check_EventFrame();
}

void CBossGroundBrust::On_Collision(const Collision& tCollision)
{

}

void CBossGroundBrust::Set_Root1()
{
    m_eType = ROOT_TYPE::ROOT1;
    m_pDynamicTexCom->Set_Texture(m_eType, 0);
    m_dwEndFrame = 53;
}

void CBossGroundBrust::Set_Root2()
{
    m_eType = ROOT_TYPE::ROOT2;
    m_pDynamicTexCom->Set_Texture(m_eType, 0);
    m_dwEndFrame = 51;
}

void CBossGroundBrust::Ready_Combat()
{
    // 히트박스 생성
    m_pColCom = CHitRectBox::Create(m_pGraphicDevice, this);
    m_pColCom->Set_Dimension({ 1.f, 2.f, 2.f });

    // 데미지 설정 
    m_tDamage.bCanParry = m_tDamage.bShouldKnockback = false;
    m_tDamage.eApplyTiming = COL_STATE::ENTER_COL;
    m_tDamage.pAttacker = m_pOwner;
    m_tDamage.dwHitTargetFlag = OBJECT_ID::PLAYER;
    m_tDamage.fAmount = 13.f;
    m_tDamage.vDirKnockback = { 0.f, 0.f, 0.f };

    m_pColCom->Set_Damage(m_tDamage);
}

void CBossGroundBrust::Ready_Texture()
{

    m_pDynamicTexCom = Add_Component<CTexture>(ID_DYNAMIC, L"Texture_Com", TEXTURE);

    vector<wstring> vecTexture = { L"Root1", L"Root2"};

    for (auto& texture : vecTexture)
        m_pDynamicTexCom->Ready_Texture(texture);
}

void CBossGroundBrust::Check_EventFrame()
{
    _uint dwFrame =  m_pDynamicTexCom->Get_CurFrame();

    if (dwFrame == m_dwEndFrame)
    {
        // Destroy 설정 
    }
}

/// <summary>
/// 생성 후 반드시 Set_Root()으로 루트 타입을 지정해줘야 한다
/// </summary>
CBossGroundBrust* CBossGroundBrust::Create(LPDIRECT3DDEVICE9 pGraphicDev, CRenderObject* pOwner)
{
    CBossGroundBrust* pObj = new CBossGroundBrust(pGraphicDev, pOwner);

    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("CBossGroundBrust Create Failed!!");
        return nullptr;
    }

    return pObj;
}

void CBossGroundBrust::Free()
{
    CRenderObject::Free();
}
