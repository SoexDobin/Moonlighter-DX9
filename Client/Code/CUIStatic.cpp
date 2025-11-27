#include "pch.h"
#include "CUIStatic.h"
#include "CRenderer.h"
#include "CTexture.h"
#include "CPrototypeManager.h"

CUIStatic::CUIStatic(LPDIRECT3DDEVICE9 pGraphicDev)
    :CRenderObject(pGraphicDev)
{
}

CUIStatic::CUIStatic(const CUIStatic& rhs)
    : CRenderObject(rhs)
{
}

CUIStatic::~CUIStatic()
{
}

HRESULT CUIStatic::Ready_GameObject()
{
    if (FAILED(CRenderObject::Ready_GameObject()))
        return E_FAIL;

    Set_RenderPriority(UI_STATIC);

    if (FAILED(UI_Initialize()))
        return E_FAIL;

    return S_OK;
}

_int CUIStatic::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CUIStatic::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CUIStatic::Render_GameObject()
{
    for (const auto& vStaticIcon : m_VecUI)
    {
        m_pTransformCom->Set_Pos(vStaticIcon.vPos.x, vStaticIcon.vPos.y, vStaticIcon.vPos.z);
        m_pTransformCom->Set_Scale(vStaticIcon.vScale.x, -vStaticIcon.vScale.y, vStaticIcon.vScale.z);

        // 갱신
        m_pTransformCom->Update_Component(0.f);

        m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        vStaticIcon.m_pTexture->Set_Texture(0); // 세팅, 바인딩
        m_pBufferCom->Render_Buffer();
    }
}
// 정보
HRESULT CUIStatic::UI_Initialize()
{
    m_VecUI.clear();
    m_VecUI.reserve(10);

   
    CComponent* pCom(nullptr);
   
    
    // 왼쪽 위
    // 코인 보따리
    {
        pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        if (!pCom || pCom->Get_ComponentType() != TEXTURE)
            return E_FAIL;

        UIElement vStaticIcon{};
        vStaticIcon.m_pTexture = static_cast<CTexture*>(pCom);
        vStaticIcon.m_pTexture->Ready_Texture(L"icon_Pouch");
        vStaticIcon.m_pTexture->Set_Texture(0, 0);
        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"icon_Pouch", vStaticIcon.m_pTexture ));

        if (vStaticIcon.m_pTexture)
        {
            vStaticIcon.vPos = _vec3(60.f, 50.f, 0.f);
            vStaticIcon.vScale = _vec3(40.f * 2.f, 40.f * 2.f, 0.f);

            m_VecUI.push_back(vStaticIcon);
        }

    }

    // HP back 아이콘
    {
       
        pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        if (!pCom || pCom->Get_ComponentType() != TEXTURE)
            return E_FAIL;

        UIElement vStaticIcon{};
        vStaticIcon.m_pTexture = static_cast<CTexture*>(pCom);
        vStaticIcon.m_pTexture->Ready_Texture(L"hp_barBack");
        vStaticIcon.m_pTexture->Set_Texture(0, 0);

        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"hp_barBack", vStaticIcon.m_pTexture));
        
        if (vStaticIcon.m_pTexture)
        {
            vStaticIcon.vPos = _vec3(209.f, 28.f, 0.f);
            vStaticIcon.vScale = _vec3(63.f * 2.f, 54.f * 2.f, 0.f);

            m_VecUI.push_back(vStaticIcon);
        }

    }
    
    // 하트 아이콘
    {
        pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        if (!pCom || pCom->Get_ComponentType() != TEXTURE)
            return E_FAIL;

        UIElement vStaticIcon{};
        vStaticIcon.m_pTexture = static_cast<CTexture*>(pCom);
        vStaticIcon.m_pTexture->Ready_Texture(L"icon_hp");
        vStaticIcon.m_pTexture->Set_Texture(0, 0);
        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"icon_hp", vStaticIcon.m_pTexture));

        if (vStaticIcon.m_pTexture)
        {
            vStaticIcon.vPos = _vec3(140.f, 28.f, 0.f);
            vStaticIcon.vScale = _vec3(25.f * 2.f, 25.f * 2.f, 0.f);

            m_VecUI.push_back(vStaticIcon);
        }

    }

    // 코인 아이콘

    {
        pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        if (!pCom || pCom->Get_ComponentType() != TEXTURE)
            return E_FAIL;

        UIElement vStaticIcon{};
        vStaticIcon.m_pTexture = static_cast<CTexture*>(pCom);
        vStaticIcon.m_pTexture->Ready_Texture(L"icon_coin");
        vStaticIcon.m_pTexture->Set_Texture(0, 0);
        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"icon_coin", vStaticIcon.m_pTexture));


        if (vStaticIcon.m_pTexture)
        {
            vStaticIcon.vPos = _vec3(35.f, 105.f, 0.f);
            vStaticIcon.vScale = _vec3(8.f * 2.f, 8.f * 2.f, 0.f);

            m_VecUI.push_back(vStaticIcon);
        }
    }


    // 오른쪽 위
   
    // 인벤토리 아이콘
    {
        pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        if (!pCom || pCom->Get_ComponentType() != TEXTURE)
            return E_FAIL;

        UIElement vStaticIcon{};
        vStaticIcon.m_pTexture = static_cast<CTexture*>(pCom);
        vStaticIcon.m_pTexture->Ready_Texture(L"icon_bag");
        vStaticIcon.m_pTexture->Set_Texture(0, 0);
        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"icon_bag", vStaticIcon.m_pTexture));
   
        if (vStaticIcon.m_pTexture)
        {
            vStaticIcon.vPos = _vec3(WINCX - 60.f, 220.f, 0.f);
            vStaticIcon.vScale = _vec3(45.f * 2.f, 45.f * 2.f, 0.f);

            m_VecUI.push_back(vStaticIcon);
        }
    }

    // 인터페이스 키 아이콘
    {
        pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
        if (!pCom || pCom->Get_ComponentType() != TEXTURE)
            return E_FAIL;

        UIElement vStaticIcon{};
        vStaticIcon.m_pTexture = static_cast<CTexture*>(pCom);
        vStaticIcon.m_pTexture->Ready_Texture(L"interface_key");
        vStaticIcon.m_pTexture->Set_Texture(0, 0);
        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"interface_key", vStaticIcon.m_pTexture));

        if (vStaticIcon.m_pTexture)
        {
            vStaticIcon.vPos = _vec3(WINCX - 100.f, 90.f, 0.f);
            vStaticIcon.vScale = _vec3(85.f * 2.f, 85.f * 2.f, 0.f);

            m_VecUI.push_back(vStaticIcon);
        }
    }

    return S_OK;
   
}

CUIStatic* CUIStatic::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    
    CUIStatic* pUIStatic = new CUIStatic(pGraphicDev);

    if (FAILED(pUIStatic->Ready_GameObject()))
    {
        Safe_Release(pUIStatic);
        MSG_BOX("UIStatic Create Falied");
        return nullptr;

    }
    return pUIStatic;
}

void CUIStatic::Free()
{
    CRenderObject::Free();
}
