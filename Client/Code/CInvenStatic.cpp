#include "pch.h"
#include "CInvenStatic.h"
#include "CManagement.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"

CInvenStatic::CInvenStatic(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr), m_bInVisible(true)
{
}

CInvenStatic::CInvenStatic(const CInvenStatic& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr), m_bInVisible(true)
{
}

CInvenStatic::~CInvenStatic()
{
}

HRESULT CInvenStatic::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTextureCom = static_cast<CTexture*>(pCom);

    m_pTextureCom->Ready_Texture(L"IconBar_Calendary_A");
    m_pTextureCom->Ready_Texture(L"IconBar_Calendary_D");
    m_pTextureCom->Ready_Texture(L"IconBar_Inventory_A");
    m_pTextureCom->Ready_Texture(L"IconBar_Inventory_D");
    m_pTextureCom->Ready_Texture(L"IconBar_Notebook_A");
    m_pTextureCom->Ready_Texture(L"IconBar_Notebook_D");
    m_pTextureCom->Ready_Texture(L"IconBar_Wishlist_A");
    m_pTextureCom->Ready_Texture(L"IconBar_Wishlist_D");

    m_pTextureCom->Set_Texture(0);

    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"InventoryStatic", m_pTextureCom));


    //m_pTransformCom->Set_Pos(m_vPanelPos.x, m_vPanelPos.y, 0.f);
    //m_pTransformCom->Set_Scale(m_fPanelWidth, -m_fPanelHeight, 0.f);


    return S_OK;
}

_int CInvenStatic::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);



    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CInvenStatic::LateUpdate_GameObject(const _float fTimeDelta)
{

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CInvenStatic::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTextureCom->Set_Texture(0); // 세팅, 바인딩
    m_pBufferCom->Render_Buffer();

}

void CInvenStatic::UI_KeyInput(const _float& fTimeDelta)
{

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_Q) & 0x80)
    {

    }

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_E) & 0x80)
    {

    }


}

void CInvenStatic::InvenStatic()
{
    m_bInVisible = !m_bInVisible;
}

CInvenStatic* CInvenStatic::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CInvenStatic* pUIStatic = new CInvenStatic(pGraphicDev);

    if (FAILED(pUIStatic->Ready_GameObject()))
    {
        MSG_BOX("UI_Menu Create Failed");
        Safe_Release(pUIStatic);
        return nullptr;
    }

    return pUIStatic;

}
void CInvenStatic::Free()
{

    Engine::CRenderObject::Free();
}
