#include "CManagement.h"
#include "CRenderer.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
    : m_pCurScene(nullptr)
{
}

CManagement::~CManagement()
{
    Free();
}

CComponent* CManagement::Get_Component(COMPONENTID eID, 
                                    const wstring pLayerTag, 
                                    const wstring pObjTag, 
                                    const wstring pComponentTag)
{
    if (nullptr == m_pCurScene)
        return nullptr;

    return m_pCurScene->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
    if (pScene == nullptr) return E_FAIL;

    Safe_Release(m_pCurScene);
    // TODO : Scene 변경 과정간 변동 제어는?
    // TODO :  씬 변경 시 릴리즈와 재사용
    m_pCurScene = pScene;

    return S_OK;
}

_int CManagement::Update_Scene(const _float fTimeDelta)
{
    if (m_pCurScene == nullptr) return -1;

    return m_pCurScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(const _float fTimeDelta)
{
    if (m_pCurScene == nullptr) return;

    m_pCurScene->LateUpdate_Scene(fTimeDelta);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    if (m_pCurScene == nullptr) return;

    CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
    m_pCurScene->Render_Scene();
}

void CManagement::Free()
{
    Safe_Release(m_pCurScene);
}