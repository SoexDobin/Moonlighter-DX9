#include "pch.h"
#include "CHpBar.h"
#include "CManagement.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"

CHpBar::CHpBar(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev),m_pTextureCom(nullptr)
{
}

CHpBar::CHpBar(const CHpBar& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr)
{
}

CHpBar::~CHpBar()
{
}

HRESULT CHpBar::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    Set_RenderPriority(UI_HPBAR);

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTextureCom = static_cast<CTexture*>(pCom);

    m_pTextureCom->Ready_Texture(L"hp_barFront");
    m_pTextureCom->Set_Texture(0);

    m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Hp_Bar", m_pTextureCom));

    m_pTransformCom->Set_Pos(fRPos, 28.f, 0.f);
    m_pTransformCom->Set_Scale(fRMax, 105.f, 0.f);



    return S_OK;
}

_int CHpBar::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

 
 
    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CHpBar::LateUpdate_GameObject(const _float fTimeDelta)
{ 
    Get_State(fTimeDelta);

    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CHpBar::Render_GameObject()
{
    
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTextureCom->Set_Texture(0); // 세팅, 바인딩
    m_pBufferCom->Render_Buffer();

}

void CHpBar::Get_State(const _float& fTimeDelta)
{
    //Hp_Max = 100.f; // 변동 가능성 있음
    
    if (iCount == Hp_Max)
    {
        MSG_BOX("Player DIed");
        return;
    }

    if (iCount < 0)
    {
        MSG_BOX("Hp Max");
        return;
    }
   
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_O) & 0x80)
    {
        iCount += 2;
        m_pTransformCom->Set_Pos(fRPos - (finter * iCount * 0.5f) , 28.f, 0.f);
        m_pTransformCom->Set_Scale(fRMax - (finter * iCount) + 1, 105.f, 0.f);
        m_bButton = true;
    }

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_P) & 0x80)
    {
        iCount -= 2;
        m_pTransformCom->Set_Pos(fRPos - (finter * iCount * 0.5f), 28.f, 0.f);
        m_pTransformCom->Set_Scale(fRMax - (finter * iCount), 105.f, 0.f);
        m_bButton = true;
    }
    m_bButton = false;

}

CHpBar* CHpBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

    CHpBar* pHpBar = new CHpBar(pGraphicDev);

    if (FAILED(pHpBar->Ready_GameObject()))
    {
        Safe_Release(pHpBar);
        MSG_BOX("UIStatic Create Falied");
        return nullptr;

    }
    return pHpBar;
}

void CHpBar::Free()
{
    CRenderObject::Free();
}
