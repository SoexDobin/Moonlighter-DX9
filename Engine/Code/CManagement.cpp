#include "CManagement.h"
#include "CRenderer.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
    : m_pCurScene(nullptr), m_iCurIndex(0)
{
}

CManagement::~CManagement()
{
    Free();
}

CComponent* CManagement::Get_Component(COMPONENTID eID, 
                                    const _tchar* pLayerTag, 
                                    const _tchar* pObjTag, 
                                    const _tchar* pComponentTag)
{
    if (nullptr == m_pCurScene)
        return nullptr;

    return m_pCurScene->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

HRESULT CManagement::Set_Scene(const _int iSceneIdx)
{
    if (iSceneIdx >= m_vecScene.size())
        return E_FAIL;

    Safe_Release(m_pCurScene);
    // TODO : Scene 변경 과정간 변동 제어는?

    m_pCurScene = m_vecScene[iSceneIdx];
    m_pCurScene->Ready_Scene();

    return S_OK;
}

HRESULT CManagement::Add_Scene(CScene* pScene)
{
    if (pScene == nullptr) return E_FAIL;
    m_vecScene.push_back(pScene);

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