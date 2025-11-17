#include "pch.h"
#include "CTestRect.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"

CTestRect::CTestRect(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTransformCom(nullptr)
{
}

CTestRect::CTestRect(const CTestRect& rhs)
    : CGameObject(rhs), m_pBufferCom(nullptr), m_pTransformCom(nullptr)
{
}

CTestRect::~CTestRect()
{
}

HRESULT CTestRect::Ready_GameObject()
{
    CComponent* pComponent;

    pComponent = Engine::CPrototypeManager::GetInstance()->Clone_Prototype(RECTCOLOR);
    if (pComponent->Get_ComponentType() != RECTCOLOR)
        return E_FAIL;
    m_pBufferCom = static_cast<CRectColor*>(pComponent);
    m_umComponent[ID_STATIC].emplace(pair<const _tchar*, CComponent*>({ L"Buffer_Com", pComponent}));

    pComponent = Engine::CPrototypeManager::GetInstance()->Clone_Prototype(TRANSFORM);
    if (pComponent->Get_ComponentType() != TRANSFORM)
        return E_FAIL;
    m_pTransformCom = static_cast<CTransform*>(pComponent);
    m_umComponent[ID_DYNAMIC].emplace(pair<const _tchar*, CComponent*>({ L"Transform_Com", pComponent }));

    return S_OK;
}

_int CTestRect::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CTestRect::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestRect::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
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