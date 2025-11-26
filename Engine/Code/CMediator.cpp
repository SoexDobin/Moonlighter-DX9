#include "CMediator.h"

CMediator::CMediator()
{
}

CMediator::~CMediator()
{
}

HRESULT CMediator::Ready_Mediator(LPDIRECT3DDEVICE9 pGraphicDev)
{
    return S_OK;
}

void CMediator::Render_Mediator()
{
    Display_AllScenePanel();
}

void CMediator::Display_AllScenePanel()
{
    bool bOpen = ImGui::Begin("Main Editor");
    if (bOpen)
    {
        ImGui::BeginChild("LeftColumn", ImVec2(200, 0), true); 
        {
            ImGui::BeginChild("Scene Type", ImVec2(0, 50), true);


            ImGui::Text("Scene : "); ImGui::SameLine();
            ImGui::PushItemWidth(80);
            if (ImGui::BeginCombo("##", m_vecScene[m_dwCurSceneIdx].c_str()))
            {
                for (_uint dwIdx = 0; dwIdx < m_dwEndScene; dwIdx++)
                {
                    _bool bSelected = m_dwCurSceneIdx == dwIdx;

                    if (ImGui::Selectable(m_vecScene[dwIdx].c_str(), bSelected))
                    {
                        m_dwCurSceneIdx = dwIdx;
                        Change_Scene(m_dwCurSceneIdx);
                    }

                    if (bSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
                ImGui::PopItemWidth();
            }
            ImGui::EndChild();

            ImGui::EndChild();
        }
        ImGui::End();

    }
}

void CMediator::Change_Scene(_uint dwSceneIdx)
{
}

void CMediator::Free()
{

}
