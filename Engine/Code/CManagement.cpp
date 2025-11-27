#include "CManagement.h"
#include "CRenderer.h"
#include "CCollisionManager.h"
#include "CLayerHelper.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
    : m_pCurScene(nullptr), m_bIsInit(true)
{
}

CManagement::~CManagement()
{
    Free();
}

CGameObject* CManagement::Get_Object(const wstring& wsLayerTag, const wstring& wsObjTag)
{
    auto itBegin = m_pCurScene->Get_Layers().at(CLayerHelper::GetInstance()->GetLayerIDByName(wsLayerTag))->Get_Objects().begin();
    auto itEnd = m_pCurScene->Get_Layers().at(CLayerHelper::GetInstance()->GetLayerIDByName(wsLayerTag))->Get_Objects().end();

    auto iter = find_if(itBegin, itEnd,
        [&wsObjTag](const pair<wstring, const list<CGameObject*>>& pair) -> _bool {
            if (pair.first == wsObjTag)
                return true;

            return false;
        });

    if (iter == itEnd) return nullptr;

    return iter->second.front();
}

CGameObject* CManagement::Get_Object(const wstring& wsObjTag)
{
    if (const list<CGameObject*>* pObj = Get_Object_List(wsObjTag))
        return pObj->front();

    return nullptr;
}

const list<CGameObject*>* CManagement::Get_Object_List(const wstring& wsObjTag)
{
    const list<CGameObject*>* pList = nullptr;
    for_each(m_pCurScene->Get_Layers().begin(), m_pCurScene->Get_Layers().end(),
        [&wsObjTag, &pList](const pair<_uint16, CLayer*>& layer) -> void {

            auto iter = find_if(layer.second->Get_Objects().begin(), layer.second->Get_Objects().end(),
                [&wsObjTag, &pList](const pair<const wstring, list<CGameObject*>>& pair) -> _bool {
                    if (pair.first == wsObjTag)
                        return true;

                    return false;
                });

            if (iter != layer.second->Get_Objects().end())
            {
                pList = &(iter->second);
                return;
            }
        });

    if (pList != nullptr) return pList;

    return nullptr;
}

CComponent* CManagement::Get_Component(COMPONENTID eID, 
                                    const wstring& wsLayerTag, 
                                    const wstring& wsObjTag, 
                                    const wstring& wsComponentTag)
{
    if (nullptr == m_pCurScene)
        return nullptr;

    return m_pCurScene->Get_Component(eID, wsLayerTag, wsObjTag, wsComponentTag);
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
    if (pScene == nullptr) return E_FAIL;

    Safe_Release(m_pCurScene);
    if (m_bIsInit)
        m_bIsInit = false;
    else
        CCollisionManager::GetInstance()->Clear_CollisionGroup();

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
