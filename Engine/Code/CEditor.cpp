#include "CEditor.h"
#include "CPanel.h"
#include "CFrameManager.h"
#include "CRenderer.h"
#include "CDInputManager.h"

IMPLEMENT_SINGLETON(CEditor)

_bool CEditor::s_bEditorActive = true;
_float CEditor::s_fEditorAlpha = 0.8f;

CEditor::CEditor()
    : m_bGamePaused(false), m_fFPS(0.f)
{
    strcpy_s(m_szPaused, "Pause");
}

CEditor::CEditor(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
    : m_bGamePaused(false)
{
    strcpy_s(m_szPaused, "Pause");
}

CEditor::~CEditor()
{
    Free();
}

HRESULT CEditor::Ready_Editor(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
#pragma region Initialize 
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.Alpha = s_fEditorAlpha;

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(pGraphicDev);
#pragma endregion

    if (ImGui::GetCurrentContext() == NULL)
    {
        MSG_BOX("Context NULL after Ready_Editor!\n");
        return E_FAIL;
    }

    return S_OK;
}

void CEditor::Render_Begin()
{
    if (!s_bEditorActive)
        return;

    // 원래 Render_Update()
    // Must be called before ImGui::Begin()
#pragma region ImGui Begin
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#pragma endregion

}

void CEditor::Render_Editor()
{
    // 에디터 on/off 임시 매핑
    {
        if (GetAsyncKeyState(VK_TAB) & 0x0001)
        {
            s_bEditorActive = !s_bEditorActive;
        }
    }

    if (!s_bEditorActive)
        return;

    Display_MainPanel();

    for (const auto& kv : m_pPanelMap)
    {
        kv.second->Display_Editor();
    }
}


void	CEditor::Render_End()
{
    if (!s_bEditorActive)
        return;

#pragma region ImGui Render
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#pragma endregion
}


void CEditor::Display_Editor(EDITORFIELD field)
{
    // Display the field according to its data type
    switch (field._type)
    {
    case Engine::DATA_BOOL:
        ImGui::Checkbox(field._label, (bool*)field._pData);
        break;

    case Engine::DATA_BYTE:
    {
        _byte* data = (_byte*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
        {
            tmp = (tmp > 128) ? 128 : (tmp < -127 ? -127 : tmp);
            *data = (_byte)tmp;
        }
        break;
    }

    case Engine::DATA_UBYTE:
    {
        unsigned char* data = (unsigned char*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
        {
            tmp = (tmp > 255) ? 255 : (tmp < 0 ? 0 : tmp);
            *data = (unsigned char)tmp;
        }
        break;
    }

    case Engine::DATA_TCHAR:
        ImGui::Text("%s : %ls", field._label, (wchar_t*)field._pData);
        break;

    case Engine::DATA_SHORT:
    {
        short* data = (short*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
        {
            tmp = (tmp > 32767) ? 32767 : (tmp < -32768 ? -32768 : tmp);
            *data = (short)tmp;
        }
        break;
    }

    case Engine::DATA_USHORT:
    {
        unsigned short* data = (unsigned short*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
        {
            tmp = (tmp > 65535) ? 65535 : (tmp < 0 ? 0 : tmp);
            *data = (unsigned short)tmp;
        }
        break;
    }

    case Engine::DATA_INT:
        ImGui::InputInt(field._label, (int*)field._pData);
        break;

    case Engine::DATA_UINT:
    {
        unsigned int* data = (unsigned int*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
        {
            *data = (tmp < 0) ? 0 : (unsigned int)tmp;
        }
        break;
    }

    case Engine::DATA_LONG:
    {
        long* data = (long*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
            *data = (long)tmp;
        break;
    }

    case Engine::DATA_ULONG:
    {
        unsigned long* data = (unsigned long*)field._pData;
        int tmp = (int)(*data);
        if (ImGui::InputInt(field._label, &tmp))
            *data = (tmp < 0) ? 0 : (unsigned long)tmp;
        break;
    }

    case Engine::DATA_FLOAT:
        ImGui::InputFloat(field._label, (float*)field._pData, 0.f, 0.f, "%.4f");
        break;

    case Engine::DATA_DOUBLE:
        ImGui::InputDouble(field._label, (double*)field._pData, 0.0, 0.0, "%.6f");
        break;

    case Engine::DATA_VEC2:
    {
        ImGui::Text(field._label);
        D3DXVECTOR2* v = (D3DXVECTOR2*)field._pData;
        ImGui::InputFloat((std::string("##") + field._label + "_x").c_str(), &v->x, 0.f, 0.f, "%.3f");
        ImGui::SameLine();
        ImGui::InputFloat((std::string("##") + field._label + "_y").c_str(), &v->y, 0.f, 0.f, "%.3f");
        break;
    }

    case Engine::DATA_VEC3:
    {
        ImGui::Text(field._label);
        D3DXVECTOR3* v = (D3DXVECTOR3*)field._pData;
        ImGui::PushItemWidth(60);
        ImGui::InputFloat((std::string("##") + field._label + "_x").c_str(), &v->x, 0.f, 0.f, "%.3f"); ImGui::SameLine();
        ImGui::InputFloat((std::string("##") + field._label + "_y").c_str(), &v->y, 0.f, 0.f, "%.3f"); ImGui::SameLine();
        ImGui::InputFloat((std::string("##") + field._label + "_z").c_str(), &v->z, 0.f, 0.f, "%.3f");
        ImGui::PopItemWidth();
        break;
    }

    case Engine::DATA_VEC4:
    {
        ImGui::Text(field._label);
        D3DXVECTOR4* v = (D3DXVECTOR4*)field._pData;
        ImGui::PushItemWidth(60);
        ImGui::InputFloat((std::string("##") + field._label + "_x").c_str(), &v->x, 0.f, 0.f, "%.3f"); ImGui::SameLine();
        ImGui::InputFloat((std::string("##") + field._label + "_y").c_str(), &v->y, 0.f, 0.f, "%.3f"); ImGui::SameLine();
        ImGui::InputFloat((std::string("##") + field._label + "_z").c_str(), &v->z, 0.f, 0.f, "%.3f"); ImGui::SameLine();
        ImGui::InputFloat((std::string("##") + field._label + "_w").c_str(), &v->w, 0.f, 0.f, "%.3f");
        ImGui::PopItemWidth();
        break;
    }

    case Engine::DATA_MAT:
    {
        ImGui::Text(field._label);
        float* mat = (float*)field._pData;
        ImGui::PushItemWidth(180);
        ImGui::InputFloat4(("##" + std::string(field._label) + "_row0").c_str(), &mat[0]);
        ImGui::InputFloat4(("##" + std::string(field._label) + "_row1").c_str(), &mat[4]);
        ImGui::InputFloat4(("##" + std::string(field._label) + "_row2").c_str(), &mat[8]);
        ImGui::InputFloat4(("##" + std::string(field._label) + "_row3").c_str(), &mat[12]);
        ImGui::PopItemWidth();

        break;
    }

    case Engine::DATA_END: default:
        break;
    }
}

// Forward declaration required
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CEditor::Editor_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

CPanel* CEditor::Create_Panel(const char* pName)
{
    if (nullptr != Find_Panel(pName))
    {
        MSG_BOX("Retry with another name");
        return nullptr;
    }

    CPanel* pPanel = CPanel::Create(pName);

    m_pPanelMap.insert({ pName, pPanel });

    return pPanel;
}

CPanel* CEditor::Get_Panel(const char* pName)
{
    return Find_Panel(pName);
}

CPanel* CEditor::Find_Panel(const char* pName)
{
    map<const char*, CPanel*>::iterator it = find_if(m_pPanelMap.begin(), m_pPanelMap.end(),
        [&](const pair<const char* const, CPanel*>& kv) ->
        bool {
            return (strcmp(pName, kv.first) == 0);
        });

    if (it == m_pPanelMap.end())
    {
        return nullptr;
    }

    return it->second;
}

void CEditor::Add_EditorField(const char* pName, EDITORFIELD pField)
{
    CPanel* pPanel = Find_Panel(pName);

    if (nullptr == pPanel)
    {
        MSG_BOX("There's no such field");
        return;
    }

    pPanel->Add_EditorField(pField);
}

void CEditor::Display_MainPanel()
{
    ImGui::Begin("Main Editor");

    ImGui::PushItemWidth(50);

    ImGui::Text("FPS : %d", CFrameManager::GetInstance()->Get_CurFPS()); ImGui::SameLine();
    ImGui::Text("DrawCall : %d", CRenderer::GetInstance()->Get_DrawCalls());

    if (ImGui::Button(m_szPaused))
    {
        if (!m_bGamePaused)
        {
            CFrameManager::GetInstance()->Pause_Game();
            m_bGamePaused = true;
            strcpy_s(m_szPaused, "Restart");
        }
        else
        {
            CFrameManager::GetInstance()->Restart_Game();
            m_bGamePaused = false;
            strcpy_s(m_szPaused, "Pause");
        }
    }
    
    if (m_bGamePaused)
    {
        ImGui::SameLine();
        if (ImGui::Button("Next Frame"))
        {
            CFrameManager::GetInstance()->Transit_NextFrame();
        }
    }

    ImGui::DragFloat("Time Scale", m_pTimeScale, 0.01f, 0.1f, 2.0f, "%.2f");

    ImGui::PopItemWidth();

    ImGui::End();
}

void CEditor::Free()
{
#pragma region ImGui Release
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#pragma endregion

    for_each(m_pPanelMap.begin(), m_pPanelMap.end(), CDeleteMap());
    m_pPanelMap.clear();
}
