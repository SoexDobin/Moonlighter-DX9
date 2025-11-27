#include "pch.h"
#include "CDInputManager.h"
#include "CPrototypeManager.h"
#include "CCameraManager.h"
#include "CTestRect.h"
#include "CManagement.h"
#include "CEditScene.h"
#include "CUtility.h"
#include "CHouse.h"
#include "CTree.h"

CEditScene::CEditScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), pVillage(nullptr), g_MapEditor(nullptr)
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
    CCameraManager::GetInstance()->Update_Camera(fTimeDelta);
    return iExit;
}

void CEditScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_Camera(fTimeDelta);
    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Add object
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("Add TerrainVillege"))
    {
        Add_TerrainVillage(L"Environment_Layer");
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

        auto iter = m_umLayer.find(L"Environment_Layer");
        if (iter != m_umLayer.end())
        {
            CLayer* pLayer = iter->second;

            CGameObject* pMapVillage = CTerrainVillage::Create(m_pGraphicDevice);

            if (FAILED(pLayer->Add_GameObject(L"TerrainVillage", pMapVillage)))
            {
                MSG_BOX("Add Terrain Village Fail");
                return;
            }
        }
        else
        {
            CLayer* pGameLogicLayer = CLayer::Create();

            CGameObject* pGameObject = nullptr;
            pGameObject = CTerrainVillage::Create(m_pGraphicDevice);
            if (FAILED(pGameLogicLayer->Add_GameObject(L"TerrainVillage", pGameObject)))
            {
                MSG_BOX("Add Terrain Village Fail");
                return;
            }

            m_umLayer.emplace(pair<const wstring, CLayer*>{ L"Environment_Layer", pGameLogicLayer});
        }
    }

    if (ImGui::Button("Add House"))
    {
        Add_House(L"Environment_Layer");
    }

    if (ImGui::Button("Add Tree"))
    {
        Add_Tree(L"Environment_Layer");
    }
    ImGui::End();

    // Layer & Object edit
    ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("Scene Object Hierarchy", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    for (const auto& layerPair : m_umLayer)
    {
        const wstring& layerName = layerPair.first;
        CLayer* pLayer = layerPair.second;

        string layerLabel = WStringToUTF8(layerName);

        if (ImGui::CollapsingHeader(layerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& objectMap = pLayer->Get_Objects();

            for (const auto& objListPair : objectMap)
            {
                const list<CGameObject*>& objectList = objListPair.second;

                for (CGameObject* pObj : objectList)
                {
                    if (!pObj)
                    {
                        MSG_BOX("Get Object Fail");
                        continue;
                    }

                    string displayName = "Unnamed";
                    if (pObj->m_szDisplayName[0] != 0)
                    {
                        wstring wname = pObj->m_szDisplayName;
                        displayName = WStringToUTF8(wname);
                    }
                    else
                    {
                        MSG_BOX("DisplayName Null");
                        return;
                    }

                    string objNodeLabel = displayName + "##" + to_string(reinterpret_cast<uintptr_t>(pObj));
                    if (ImGui::TreeNode(objNodeLabel.c_str()))
                    {
                        CTransform* pTrans = static_cast<CRenderObject*>(pObj)->Get_Trans();

                        if (pTrans)
                        {
                            _vec3 vPos = pTrans->Get_Pos();
                            _vec3 vScale = pTrans->Get_Scale();

                            if (ImGui::DragFloat3("Position", reinterpret_cast<float*>(&vPos), 0.1f))
                            {
                                pTrans->Set_Pos(vPos.x, vPos.y, vPos.z);
                            }

                            if (ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&vScale), 0.1f, 0.1f, 100.f))
                            {
                                pTrans->Set_Scale(vScale.x, vScale.y, vScale.z);
                            }
                        }
                        else
                        {
                            MSG_BOX("Get Transform Fail");
                            return;
                        }

                        ImGui::TreePop();
                    }
                }
            }
        }
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
    CCameraManager::GetInstance()->Set_Target(nullptr);
    CCameraManager::GetInstance()->Set_CameraMode(CCameraManager::DBG_PERSPECTIVE);

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

HRESULT CEditScene::Add_TerrainVillage(const wstring pLayerTag)
{
    auto iter = m_umLayer.find(pLayerTag);
    if (iter != m_umLayer.end())
    {
        CLayer* pLayer = iter->second;

        CGameObject* pHouse = CTerrainVillage::Create(m_pGraphicDevice);

        if (FAILED(pLayer->Add_GameObject(L"Village", pHouse)))
        {
            MSG_BOX("Add Village Fail");
            return E_FAIL;
        }
    }
    else
    {
        CLayer* pGameLogicLayer = CLayer::Create();

        CGameObject* pGameObject = nullptr;
        pGameObject = CTerrainVillage::Create(m_pGraphicDevice);
        if (FAILED(pGameLogicLayer->Add_GameObject(L"Village", pGameObject)))
        {
            MSG_BOX("Add Village Fail");
            return E_FAIL;
        }

        m_umLayer.emplace(pair<const wstring, CLayer*>{ pLayerTag, pGameLogicLayer});
    }
    return S_OK;
}

HRESULT CEditScene::Add_House(const wstring pLayerTag)
{
    auto iter = m_umLayer.find(pLayerTag);
    if (iter != m_umLayer.end())
    {
        CLayer* pLayer = iter->second;

        CGameObject* pHouse = CHouse::Create(m_pGraphicDevice);

        if (pHouse != nullptr)
        {
            _int randA, randB;

            srand(_uint(time(nullptr)));

            randA = rand() % 100 + 1;
            randB = rand() % 100 + 1;

            static_cast<CRenderObject*>(pHouse)->Get_Trans()->Set_Pos(_float(randA), 4.f, _float(randB));
        }
        else
        {
            MSG_BOX("House Create Fail");
            return E_FAIL;
        }

        if (FAILED(pLayer->Add_GameObject(L"House", pHouse)))
        {
            MSG_BOX("Add House Fail");
            return E_FAIL;
        }
    }
    else
    {
        CLayer* pGameLogicLayer = CLayer::Create();

        CGameObject* pGameObject = nullptr;
        pGameObject = CHouse::Create(m_pGraphicDevice);
        if (pGameObject != nullptr)
        {
            _int randA, randB;

            srand(_uint(time(nullptr)));

            randA = rand() % 100 + 1;
            randB = rand() % 100 + 1;

            static_cast<CRenderObject*>(pGameObject)->Get_Trans()->Set_Pos(_float(randA), 4.f, _float(randB));
        }
        if (FAILED(pGameLogicLayer->Add_GameObject(L"House", pGameObject)))
            return E_FAIL;

        m_umLayer.emplace(pair<const wstring, CLayer*>{ pLayerTag , pGameLogicLayer});
    }

    return S_OK;
}

HRESULT CEditScene::Add_Tree(const wstring pLayerTag)
{
    auto iter = m_umLayer.find(pLayerTag);
    if (iter != m_umLayer.end())
    {
        CLayer* pLayer = iter->second;

        CGameObject* pHouse = CTree::Create(m_pGraphicDevice);

        if (pHouse != nullptr)
        {
            _int randA, randB;

            srand(_uint(time(nullptr)));

            randA = rand() % 100 + 1;
            randB = rand() % 100 + 1;

            static_cast<CRenderObject*>(pHouse)->Get_Trans()->Set_Pos(_float(randA), 4.f, _float(randB));
        }
        else
        {
            MSG_BOX("House Create Fail");
            return E_FAIL;
        }

        if (FAILED(pLayer->Add_GameObject(L"House", pHouse)))
        {
            MSG_BOX("Add House Fail");
            return E_FAIL;
        }
    }
    else
    {
        CLayer* pGameLogicLayer = CLayer::Create();

        CGameObject* pGameObject = nullptr;
        pGameObject = CTree::Create(m_pGraphicDevice);
        if (pGameObject != nullptr)
        {
            _int randA, randB;

            srand(_uint(time(nullptr)));

            randA = rand() % 100 + 1;
            randB = rand() % 100 + 1;

            static_cast<CRenderObject*>(pGameObject)->Get_Trans()->Set_Pos(_float(randA), 4.f, _float(randB));
        }
        if (FAILED(pGameLogicLayer->Add_GameObject(L"House", pGameObject)))
            return E_FAIL;

        m_umLayer.emplace(pair<const wstring, CLayer*>{ pLayerTag, pGameLogicLayer});
    }

    return S_OK;
}


inline string CEditScene::WStringToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(),nullptr, 0, nullptr, nullptr);

    string strTo(size_needed, 0);

    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(),&strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}
