#include "pch.h"
#include "CTestRect.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"

CTestRect::CTestRect(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTexCom(nullptr)
{
}

CTestRect::CTestRect(const CTestRect& rhs)
    : CRenderObject(rhs), m_pTexCom(nullptr)
{
}

CTestRect::~CTestRect()
{
}

HRESULT CTestRect::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = Engine::CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTexCom = static_cast<CTexture*>(pCom))
    {
        m_pTexCom->Ready_Texture(L"Player_Roll", 8);
    }

    m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });

    return S_OK;
}

_int CTestRect::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CTestRect::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestRect::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTexCom->Set_Texture(0, 0);
    m_pBufferCom->Render_Buffer();
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