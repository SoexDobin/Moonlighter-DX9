#include "pch.h"
#include "CDInputManager.h"
#include "CResourceManager.h"
#include "CPrototypeManager.h"
#include "CDynamicCamera.h"
#include "CTestRect.h"
#include "CManagement.h"
#include "CEditScene.h"
#include "CUtility.h"
#include "CHouse.h"
#include "CTree.h"

CEditScene::CEditScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), pVillage(nullptr), g_MapEditor(nullptr), g_pPreviewTex(nullptr)
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

    // mouse coordinates
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        ImVec2 mousePos = ImGui::GetMousePos();

        find_VillageTerrain();

        _vec3 vMousePos = CUtility::Picking_Terrain(m_pGraphicDevice, g_hWnd, static_cast<CTerrainVillage*>(pVillage));

        ImGui::SetNextWindowPos(ImVec2(mousePos.x + 15, mousePos.y + 15));
        ImGui::SetNextWindowBgAlpha(0.5f);
        ImGui::Begin("Mouse Pos", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav);
        ImGui::Text("X: %.0f", vMousePos.x);
        ImGui::Text("Z: %.0f", vMousePos.z);
        ImGui::End();
    }

    // Add object
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Village"))
    {
        if (ImGui::Button("Add TerrainVillage"))
        {
            Add_TerrainVillage(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Village");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Save Village Terrain"))
        {
            find_VillageTerrain();
            CUtility::SaveVillageMap(static_cast<CTerrainVillage*>(pVillage), m_umLayer);
        }

        if (ImGui::Button("Load Village Terrain"))
        {
            CUtility::LoadVillageMap(m_pGraphicDevice, m_umLayer);
        }

        if (ImGui::Button("Add House"))
        {
            Add_House(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Village_House");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Add Tree"))
        {
            Add_Tree(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Village_Tree");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }
    }

    if (ImGui::CollapsingHeader("Dungeon"))
    {
        if (ImGui::Button("Add Terrain Dungeon"))
        {
            Add_TerrainDungeon(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Dungeon");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }
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

        CGameObject* pGameObject = CTerrainVillage::Create(m_pGraphicDevice);

        if (FAILED(pLayer->Add_GameObject(L"Village", pGameObject)))
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

HRESULT CEditScene::Add_TerrainDungeon(const wstring pLayerTag)
{
    auto iter = m_umLayer.find(pLayerTag);
    if (iter != m_umLayer.end())
    {
        CLayer* pLayer = iter->second;

        CGameObject* pGameObject = CTerrainDungeonNormal::Create(m_pGraphicDevice);

        if (FAILED(pLayer->Add_GameObject(L"Terrain_Dungeon", pGameObject)))
        {
            MSG_BOX("Add Dungeon Fail");
            return E_FAIL;
        }
    }
    else
    {
        CLayer* pGameLogicLayer = CLayer::Create();

        CGameObject* pGameObject = nullptr;
        pGameObject = CTerrainDungeonNormal::Create(m_pGraphicDevice);
        if (FAILED(pGameLogicLayer->Add_GameObject(L"Terrain_Dungeon", pGameObject)))
        {
            MSG_BOX("Add Dungeon Fail");
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

    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1 ,&strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

void CEditScene::InitPreviewTextures(const wstring wPreview)
{
    auto pSprite = CResourceManager::GetInstance()->Get_Sprite(wPreview);
    if (pSprite[0])
        g_pPreviewTex = pSprite[0];
}

void CEditScene::find_VillageTerrain()
{
    auto iter = m_umLayer.find(L"Environment_Layer");
    if (iter != m_umLayer.end())
    {
        CLayer* pLayer = iter->second;
        const auto& objMap = pLayer->Get_Objects();
        for (const auto& objList : objMap)
        {
            for (CGameObject* pObj : objList.second)
            {
                if (wcscmp(pObj->m_szDisplayName, L"Terrain_Village") == 0)
                {
                    pVillage = static_cast<CTerrainVillage*>(pObj);
                    break;
                }
            }
        }
    }
}
