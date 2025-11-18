#include "pch.h"
#include "CTestRect.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"

CTestRect::CTestRect(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTexCom(nullptr), m_fDelta(0)
{
}

CTestRect::CTestRect(const CTestRect& rhs)
    : CRenderObject(rhs), m_pTexCom(nullptr), m_fDelta(0)
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
        m_pTexCom->Ready_Texture(L"Item_Potion", 4);
        m_pTexCom->Ready_Texture(L"Player_Roll", 8);
    }
    
    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pTexCom));

    return S_OK;
}

_int CTestRect::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    m_fDelta += 10.f * fTimeDelta;
    //if (m_fDelta > m_pTexCom->Get_FrameCount(0))
    //    m_fDelta = 0.f;

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

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTexCom->Set_Texture(0, 0);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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