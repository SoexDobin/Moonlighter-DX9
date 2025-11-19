#include "CScene.h"
#include "CManagement.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDevice(pGraphicDev), m_iSceneIdx(0)
{
    m_pGraphicDevice->AddRef();
}

CScene::~CScene()
{

}

CComponent* CScene::Get_Component(COMPONENTID eID, const wstring& wsLayerTag, const wstring& wsObjTag, const wstring& wsComponentTag)
{
    auto iter = find_if(m_umLayer.begin(), m_umLayer.end()
        , [&wsLayerTag](pair<const std::wstring, CLayer*>& pair) -> _bool {
            if (pair.first == wsLayerTag)
                return true;

            return false;
        });

    if (iter == m_umLayer.end()) return nullptr;

    return iter->second->Get_Component(eID, wsObjTag, wsComponentTag);
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

_int CScene::Update_Scene(const _float fTimeDelta)
{
    for_each(m_umLayer.begin(), m_umLayer.end()
        , [fTimeDelta](pair<const wstring, CLayer*>& pair) -> void {
            pair.second->Update_Layer(fTimeDelta);
        });
    return 0;
}

void CScene::LateUpdate_Scene(const _float fTimeDelta)
{
    for_each(m_umLayer.begin(), m_umLayer.end()
        , [fTimeDelta](pair<const wstring, CLayer*>& pair) -> void {
            pair.second->LateUpdate_Layer(fTimeDelta);
        });
}

void CScene::Render_Scene()
{
    for_each(m_umLayer.begin(), m_umLayer.end()
        , [](pair<const wstring, CLayer*>& pair) -> void {
            pair.second->Render_Layer();
        });

    Display_Editor();
}

void CScene::Free()
{
    for_each(m_umLayer.begin(), m_umLayer.end(), CDeleteMap());
    m_umLayer.clear();

    Safe_Release(m_pGraphicDevice);
}

void CScene::Display_Editor()
{
    ImGui::Begin("Scene");

    _uint dwIndex = 0;
    for (auto& layer : m_umLayer)
    {
        ImGui::Checkbox(("##" + to_string(dwIndex++)).c_str(), &layer.second->m_bDisplayInEditor);       ImGui::SameLine();
        
        ImGui::Text("%ls", layer.first.c_str());
        
        layer.second->Display_Editor();
    }

    ImGui::End();
}
