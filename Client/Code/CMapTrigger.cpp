#include "pch.h"
#include "CMapTrigger.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

CMapTrigger::CMapTrigger(LPDIRECT3DDEVICE9 pGraphicDev) : CRenderObject(pGraphicDev), m_pRectCollider(nullptr), eScene(SC_END)
{
}

CMapTrigger::CMapTrigger(const CMapTrigger& rhs) : CRenderObject(rhs.m_pGraphicDevice), m_pRectCollider(nullptr), eScene(SC_END)
{
}

CMapTrigger::~CMapTrigger()
{
}

HRESULT CMapTrigger::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    if (m_pRectCollider = Add_Component<CRectCollider>(ID_DYNAMIC, L"Collider_Com", RECT_COLLIDER))
    {
        m_pRectCollider->Set_Dimension({4.f, 8.f, 4.f});
    }
    return S_OK;
}

_int CMapTrigger::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);
    return iExit;
}

void CMapTrigger::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapTrigger::Render_GameObject()
{

}

CMapTrigger* CMapTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMapTrigger* pInstance = new CMapTrigger(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CMapTrigger Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CMapTrigger::Free()
{
    Engine::CGameObject::Free();
}
