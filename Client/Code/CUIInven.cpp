#include "pch.h"
#include "CUIInven.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"

CUIInven::CUIInven(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr), m_bVisible(false)
{
}

CUIInven::CUIInven(const CUIInven& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr), m_bVisible(false)
{
}

CUIInven::~CUIInven()
{
}

HRESULT CUIInven::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr); 

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTextureCom = static_cast<CTexture*>(pCom);
   
    m_pTextureCom->Ready_Texture(L"Inventory_Base");
    m_pTextureCom->Set_Texture(0, 0);

    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Inventory_Base", m_pTextureCom));

    m_pTransformCom->Set_Scale(450.f, -480.f, 0.f);
    m_pTransformCom->Set_Pos(WINCX * 0.5f, WINCY * 0.5f, 0.f);



    return S_OK;
}

_int CUIInven::Update_GameObject(const _float fTimeDelta)
{
    if (!m_bVisible)
    {
        return 0;
    }

    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
    

    return iExit;
}

void CUIInven::LateUpdate_GameObject(const _float fTimeDelta)
{
    if (!m_bVisible)
    {
        return;
    }

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);

    
}

void CUIInven::Render_GameObject()
{
    if (!m_bVisible)
    {
        return;
    }

    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTextureCom->Set_Texture(0); // 세팅, 바인딩(
    m_pBufferCom->Render_Buffer();
}

void CUIInven::UI_KeyInput(const _float& fTimeDelta)
{

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_Q) & 0x80 )
    {

    }

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_E) & 0x80)
    {

    }


}



void CUIInven::InvenButton()
{
   m_bVisible =  !m_bVisible;
}

CUIInven* CUIInven::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUIInven* pUIMenu = new CUIInven(pGraphicDev);

    if (FAILED(pUIMenu->Ready_GameObject()))
    {
        MSG_BOX("UI_Menu Create Failed");
        Safe_Release(pUIMenu);
        return nullptr;
    }

    return pUIMenu;
}
void CUIInven::Free()
{
    Engine::CRenderObject::Free();
}
