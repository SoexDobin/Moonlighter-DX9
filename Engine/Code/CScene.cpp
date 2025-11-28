#include "CScene.h"
#include "CManagement.h"
#include "CLayerHelper.h"
#include "CEditor.h"


CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDevice(pGraphicDev)
{
    m_pGraphicDevice->AddRef();
}

CScene::~CScene()
{

}

CLayer* CScene::Get_Layer(_uint16 eID) { return m_umLayer[eID]; }
CLayer* CScene::Get_Layer(const wstring& wsLayerTag)
{
    LAYERID eID = CLayerHelper::GetInstance()->GetLayerIDByName(wsLayerTag);
    return m_umLayer[eID];
}

CComponent* CScene::Get_Component(COMPONENTID eID, const wstring& wsLayerTag, const wstring& wsObjTag, const wstring& wsComponentTag)
{
    auto iter = find_if(m_umLayer.begin(), m_umLayer.end()
        , [&wsLayerTag](const pair<_uint16, CLayer*>& pair) -> _bool {
            if (pair.first == (pair.first & CLayerHelper::GetInstance()->GetLayerIDByName(wsLayerTag)))
                return true;

            return false;
        });

    if (iter == m_umLayer.end()) return nullptr;

    return iter->second->Get_Component(eID, wsObjTag, wsComponentTag);
}

HRESULT CScene::Ready_Scene()
{
    m_umLayer.clear();

    for (_uint16 i = 1; i != LAYER_NONE; i = i << 1)
    {
        CLayer* pLayer = CLayer::Create(Engine::CLayerHelper::GetInstance()->GetLayerNameByID(LAYERID(i)));
        m_umLayer.emplace(pair<_uint16, CLayer*>(i, pLayer));
    }

    return S_OK;
}

_int CScene::Update_Scene(const _float fTimeDelta)
{
    for (_uint16 i = LAYER0; i != LAYER_NONE; i = i << 1)
    {
        m_umLayer[i]->Update_Layer(fTimeDelta);
    }

    return 0;
}

void CScene::LateUpdate_Scene(const _float fTimeDelta)
{
    for (_uint16 i = LAYER0; i != LAYER_NONE; i = i << 1)
    {
        m_umLayer[i]->LateUpdate_Layer(fTimeDelta);
    }
}

void CScene::Render_Scene()
{
    for (_uint16 i = LAYER0; i != LAYER_NONE; i = i << 1)
    {
        m_umLayer[i]->Render_Layer();
    }

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
    bool bOpen = ImGui::Begin("Main Editor");
    if (bOpen)
    {
        ImGui::BeginChild("LeftColumn", ImVec2(200, 0), true); 
        {
            ImGui::BeginChild("Scene Type", ImVec2(0, 50), true);
            { ImGui::EndChild(); }

            ImGui::BeginChild("Scene Info", ImVec2(0, 0), true);
            {
                ImGui::PushItemWidth(50);
                for (auto& layer : m_umLayer)
                {
                    ImGuiTreeNodeFlags flags =
                        ImGuiTreeNodeFlags_SpanAvailWidth |
                        ImGuiTreeNodeFlags_DefaultOpen |
                        ImGuiTreeNodeFlags_FramePadding;

                    if (ImGui::TreeNodeEx(layer.second->m_LayerTag, flags))
                    {
                        layer.second->Display_Editor();
                        ImGui::TreePop();
                    }
                }
                ImGui::PopItemWidth();
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    ImGui::End();

}
