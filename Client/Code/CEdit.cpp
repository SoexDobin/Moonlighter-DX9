#include "pch.h"
#include "CEdit.h"
#include "CPrototypeManager.h"
#include "CDynamicCamera.h"

CEdit::CEdit(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CScene(pGraphicDev)
{

}


CEdit::~CEdit()
{

}

HRESULT CEdit::Ready_Scene()
{
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
	return S_OK;
}

_int CEdit::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CEdit::LateUpdate_Scene(const _float fTimeDelta)
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

}

void CEdit::Render_Scene()
{
    // Rendering
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

HRESULT CEdit::Ready_Environment_Layer(const wstring pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;


    _vec3   vEye{ 0.f, 10.f, -10.f };
    _vec3   vAt{ 0.f, 0.f, 1.f };
    _vec3   vUp{ 0.f, 1.f, 0.f };

    // dynamicCamera
    pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
        return E_FAIL;

    // skybox
    //pGameObject = CSkyBox::Create(m_pGraphicDev);

    //if (nullptr == pGameObject)
    //    return E_FAIL;

    //if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
    //    return E_FAIL;


    m_umLayer.insert({ pLayerTag , pLayer });            // scene -> map
	return S_OK;
}

HRESULT CEdit::Ready_GameLogic_Layer(const wstring pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_umLayer.insert({ pLayerTag , pLayer });
    return S_OK;
}

HRESULT CEdit::Ready_UI_Layer(const wstring pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;




    m_umLayer.insert({ pLayerTag , pLayer });
    return S_OK;
}

HRESULT CEdit::Ready_Prototype()
{
    if (FAILED(CPrototypeManager::GetInstance()->Ready_Prototype(RECTCOLOR, Engine::CRectColor::Create(m_pGraphicDevice))))
        return E_FAIL;
    return S_OK;
}

CEdit* CEdit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEdit* pEdit = new CEdit(pGraphicDev);

    if (FAILED(pEdit->Ready_Scene()))
    {
        MSG_BOX("Stage Create Failed");
        Safe_Release(pEdit);
        return nullptr;
    }

    return pEdit;
}

void CEdit::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
	Engine::CScene::Free();
}