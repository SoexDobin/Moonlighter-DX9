#include "pch.h"
#include "CDInputManager.h"
#include "CPrototypeManager.h"
#include "CDynamicCamera.h"
#include "CTestRect.h"
#include "CManagement.h"
#include "CEditScene.h"
#include "CUtility.h"

CEditScene::CEditScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), pVillage(nullptr)
{
}

CEditScene::~CEditScene()
{
}

HRESULT CEditScene::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Camera_Layer(L"Camera_Layer")))
        return E_FAIL;
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;
    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;
    if (FAILED(Ready_UI_Layer(L"Ui_Layer")))
        return E_FAIL;

    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    g_MapEditor = ImGui::CreateContext();

    //ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDevice);

     Sleep(100);

    return S_OK;
}

_int CEditScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CEditScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("Add TerrainVillege"))
    {
        Add_TerrainVillage(L"GameLogic_Layer");
    }
    if (ImGui::Button("Save Terrain"))
    {
        CUtility::SaveMap(static_cast<CTerrainVillage*>(pVillage));
    }
    if (ImGui::Button("Load Terrain"))
    {
        _ulong pCntX, pCntZ, pInterval;
        wstring heightMap;

        CUtility::LoadMap(&pCntX, &pCntZ, &pInterval, heightMap);

        CLayer* pGameLogicLayer = CLayer::Create();

        CGameObject* pGameObject = nullptr;
        pGameObject = CTerrainVillage::Create(m_pGraphicDevice);
        if (FAILED(pGameLogicLayer->Add_GameObject(L"TerrainVillage", pGameObject)))
            return;

        m_umLayer.emplace(pair<const wstring, CLayer*>{ L"GameLogic_Layer", pGameLogicLayer});
    }
    ImGui::End();
}

void CEditScene::Render_Scene()
{
    Engine::CScene::Render_Scene();

    // Rendering
    ImGui::SetCurrentContext(g_MapEditor);
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

HRESULT CEditScene::Ready_Camera_Layer(const wstring wsLayerTag)
{
    CLayer* pCamLayer = CLayer::Create();

    CGameObject* pGameObject = nullptr;
    _vec3 vEye{ 0.f, 10.f, -10.f }, vAt{ 0.f, 0.f, 10.f }, vUp{ 0.f, 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);
    if (FAILED(pCamLayer->Add_GameObject(L"Cam", pGameObject)))
        return E_FAIL;

    m_umLayer.emplace(pair<const wstring, CLayer*>{ wsLayerTag, pCamLayer});
    return S_OK;
}

HRESULT CEditScene::Ready_Environment_Layer(const wstring wsLayerTag)
{
    return S_OK;
}

HRESULT CEditScene::Ready_GameLogic_Layer(const wstring wsLayerTag)
{
    return S_OK;
}

HRESULT CEditScene::Ready_UI_Layer(const wstring wsLayerTag)
{
    return S_OK;
}

HRESULT CEditScene::Ready_Prototype()
{
    //if (FAILED(CPrototypeManager::GetInstance()->Ready_Prototype(TERRAINTEX, Engine::CTerrainTex::Create(m_pGraphicDevice, 64, 64, 1, L""))))
    //{
    //    MSG_BOX("Edit Scene CTerrainTex Prototype Create Error");
    //    return E_FAIL;
    //}

    return S_OK;
}

CEditScene* CEditScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEditScene* pLogo = new CEditScene(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("MainScene Create Failed");
        Safe_Release(pLogo);
        return nullptr;
    }

    return pLogo;
}

void CEditScene::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    Engine::CScene::Free();
}

HRESULT CEditScene::Add_TerrainVillage(const wstring temp)
{
    CLayer* pGameLogicLayer = CLayer::Create();

    CGameObject* pGameObject = nullptr;
    pGameObject = CTerrainVillage::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"TerrainVillage", pGameObject)))
        return E_FAIL;

    m_umLayer.emplace(pair<const wstring, CLayer*>{ temp, pGameLogicLayer});
    pVillage = pGameObject;
    return S_OK;
}
