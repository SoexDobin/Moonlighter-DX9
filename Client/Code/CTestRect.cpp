#include "pch.h"
#include "CTestRect.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"
#include "CSphereCollider.h"
#include "CCollisionManager.h"

CTestRect::CTestRect(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pDynamicTexCom(nullptr), m_pStaticTexCom(nullptr)
{
    PANEL_NAME(L"Rect Object");
}

CTestRect::CTestRect(const CTestRect& rhs)
    : CRenderObject(rhs), m_pDynamicTexCom(nullptr), m_pStaticTexCom(nullptr)
{
    PANEL_NAME(L"Rect Object");
}

CTestRect::~CTestRect()
{
    PANEL_NAME(L"Rect Object");
}

HRESULT CTestRect::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    
    if (m_pDynamicTexCom = Add_Component<CTexture>(ID_DYNAMIC, L"Texture_Com", TEXTURE))
    {
        m_pDynamicTexCom->Set_Speed(10.f);
        m_pDynamicTexCom->Ready_Texture(L"Item_Potion");
        m_pDynamicTexCom->Ready_Texture(L"Player_Roll");
        m_pDynamicTexCom->Set_Texture(POTION);
    }

    //m_pColCom = Add_Component<CSphereCollider>(ID_DYNAMIC, L"Collider_Com", SPHERE_COLLIDER);

    //pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    //if (pCom->Get_ComponentType() != TEXTURE)
    //    return E_FAIL;
    //if (m_pStaticTexCom = static_cast<CTexture*>(pCom))
    //{
    //    m_pStaticTexCom->Ready_Texture(L"Item_Potion");
    //    m_pStaticTexCom->Set_Texture(POTION, 2);
    //    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pStaticTexCom));
    //}

    return S_OK;
}

_int CTestRect::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_9))
    {
        m_pDynamicTexCom->Set_Texture(POTION);
    }
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_0))
    {
        m_pDynamicTexCom->Set_Texture(ROLL);
    }

    return iExit;
}

void CTestRect::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestRect::Render_GameObject()
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

void CTestRect::On_Collision(const Collision& tCollision)
{
    int a = 0;
}

CTestRect* CTestRect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTestRect* pInstance = new CTestRect(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CTestRect Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CTestRect::Free()
{
    Engine::CGameObject::Free();
}
