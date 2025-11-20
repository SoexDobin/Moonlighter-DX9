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

// 메인앱에서 CEditor 다음에 호출하자 
void CMediator::Render_Mediator()
{
    Display_AllScenePanel();
}

void CMediator::Display_AllScenePanel()
{
    if (ImGui::BeginTabBar("Debugging"))
    {
        ImGui::PushItemWidth(80);
        if (ImGui::BeginTabItem("All Scenes"))
        {
            if (ImGui::BeginCombo(": Scene Type", m_vecScene[m_dwCurSceneIdx].c_str()))
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
            }
            ImGui::EndTabItem();
        }

        ImGui::PopItemWidth();
        ImGui::EndTabBar();
    }
}

void CMediator::Change_Scene(_uint dwSceneIdx)
{
}

void CMediator::Free()
{

}
