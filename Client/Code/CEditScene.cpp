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
#include "CTreeObject.h"
#include "CDungeonWall.h"
#include "CBossWallFront.h"
#include "CBossWallSide.h"
#include "CBossWallFrontUpper.h"
#include "CBossWallSideUpper.h"
#include "CPumpkin.h"

CEditScene::CEditScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), pVillage(nullptr), g_MapEditor(nullptr), g_pPreviewTex(nullptr),
    m_pSelectedObject(nullptr), bDragging(false), pDragObject(nullptr), isReadyPick(false)
{
    ZeroMemory(&confirm, sizeof(confirm));
    ZeroMemory(&vRot, sizeof(_vec3));
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

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
        ImGui::Text("X: %.3f", vMousePos.x);
        ImGui::Text("Z: %.3f", vMousePos.z);
        ImGui::End();
    }

    // Add object
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Checkbox("Object Picking Mode", &isReadyPick);

    if (ImGui::CollapsingHeader("Village"))
    {
        if (ImGui::Button("Save Village Terrain"))
        {
            confirm[VILL_SAVE] = true;
            ImGui::OpenPopup("Confirm Village Save");
        }
        if (ImGui::BeginPopupModal("Confirm Village Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to save?\nThis operation cannot be undone.");
            ImGui::Separator();

            if (ImGui::Button("yes", ImVec2(120, 0)))
            {
                find_VillageTerrain();
                CUtility::SaveVillageMap(static_cast<CTerrainVillage*>(pVillage), m_umLayer);
                ImGui::CloseCurrentPopup();
                confirm[VILL_SAVE] = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("no", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                confirm[VILL_SAVE] = false;
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Load Village Terrain"))
        {
            confirm[VILL_LOAD] = true;
            ImGui::OpenPopup("Confirm Village Load");
        }
        if (ImGui::BeginPopupModal("Confirm Village Load", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to load?\nThis operation cannot be undone.");
            ImGui::Separator();

            if (ImGui::Button("yes", ImVec2(120, 0)))
            {
                CUtility::LoadVillageMap(m_pGraphicDevice, m_umLayer);
                ImGui::CloseCurrentPopup();
                confirm[VILL_LOAD] = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("no", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                confirm[VILL_LOAD] = false;
            }

            ImGui::EndPopup();
        }

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

        if (ImGui::Button("Add Dungeon Wall"))
        {
            Add_DungeonWall(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Dungeon_Wall");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }
    }

    if (ImGui::CollapsingHeader("Boss"))
    {
        if (ImGui::Button("Add Terrain Boss"))
        {
            Add_TerrainBoss(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Boss");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Add Boss Wall Front"))
        {
            Add_BossWallFront_Down(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Boss_Wall_Down");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Add Boss Wall Side"))
        {
            Add_BossWallSide_Down(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Boss_Wall_Side");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Add Boss Wall Front Upper"))
        {
            Add_BossWallFront_Up(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Boss_Wall_Up");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Add Boss Wall Side Upper"))
        {
            Add_BossWallSide_Up(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Boss_Wall_Up");
            ImGui::BeginTooltip();
            ImGui::Image((ImTextureID)g_pPreviewTex, ImVec2(64, 64));
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Add Pumpkin"))
        {
            Add_Pumpkin(L"Environment_Layer");
        }
        if (ImGui::IsItemHovered())
        {
            InitPreviewTextures(L"Map_Boss_Pumpkin");
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
                            _vec3 vCurrentAngle = pTrans->Get_Angle();
                            ImGui::Text("Current Rotation: X=%.2f, Y=%.2f, Z=%.2f", vCurrentAngle.x, vCurrentAngle.y, vCurrentAngle.z);

                            if (ImGui::DragFloat3("Rotation (Pitch/Yaw/Roll)", reinterpret_cast<float*>(&vRot), 0.1f))
                            {
                                pTrans->Add_Rotation(ROT_X, vRot.x);
                                pTrans->Add_Rotation(ROT_Y, vRot.y);
                                pTrans->Add_Rotation(ROT_Z, vRot.z);

                                vRot = { 0.f, 0.f, 0.f };
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

    if (isReadyPick)
    {
        find_VillageTerrain();
        if (!pVillage)
        {
            return;
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (!ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            {
                _vec3 vPickPos = CUtility::Picking_Terrain(
                    m_pGraphicDevice,
                    g_hWnd,
                    static_cast<CTerrainVillage*>(pVillage));

                CGameObject* pPicked = PickObject(vPickPos);
                if (pPicked)
                {
                    pDragObject = pPicked;
                    bDragging = true;
                }
            }
        }
        if (bDragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            _vec3 vDropPos = CUtility::Picking_Terrain(
                m_pGraphicDevice,
                g_hWnd,
                static_cast<CTerrainVillage*>(pVillage));

            if (pDragObject)
            {
                CTransform* pTrans = static_cast<CRenderObject*>(pDragObject)->Get_Trans();
                if (pTrans)
                {
                    _vec3 vOldPos = pTrans->Get_Pos();
                    pTrans->Set_Pos(vDropPos.x, vOldPos.y, vDropPos.z);
                }
            }

            pDragObject = nullptr;
            bDragging = false;
        }
        if (bDragging && pDragObject)
        {
            _vec3 vDragPos = CUtility::Picking_Terrain(
                m_pGraphicDevice, g_hWnd, static_cast<CTerrainVillage*>(pVillage));
            CTransform* pTrans = static_cast<CRenderObject*>(pDragObject)->Get_Trans();
            if (pTrans)
            {
                _vec3 vOldPos = pTrans->Get_Pos();
                pTrans->Set_Pos(vDragPos.x, vOldPos.y, vDragPos.z);
            }
        }
    }
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
    if (FAILED(Add_ObjectToLayer<CTerrainVillage>(this, pLayerTag, L"Terrain_Village")))
    {
        MSG_BOX("Terrain Village Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_TerrainDungeon(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CTerrainDungeonNormal>(this, pLayerTag, L"Terrain_Dungeon")))
    {
        MSG_BOX("Terrain Dungeon Add Fail");
        return E_FAIL;
    }
    return S_OK;
}


HRESULT CEditScene::Add_House(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CHouse>(this, pLayerTag, L"Village_House")))
    {
        MSG_BOX("House Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_Tree(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CTreeObject>(this, pLayerTag, L"Village_Tree")))
    {
        MSG_BOX("Tree Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_DungeonWall(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CDungeonWall>(this, pLayerTag, L"Dungeon_Wall")))
    {
        MSG_BOX("DungeonWall Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_TerrainBoss(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CTerrainBoss>(this, pLayerTag, L"Terrain_Boss")))
    {
        MSG_BOX("Terrain Boss Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_BossWallFront_Down(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CBossWallFront>(this, pLayerTag, L"Boss_Wall_Front")))
    {
        MSG_BOX("Boss Wall Front Down Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_BossWallSide_Down(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CBossWallSide>(this, pLayerTag, L"Boss_Wall_Side")))
    {
        MSG_BOX("Boss Wall Side Down Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_BossWallFront_Up(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CBossWallFrontUpper>(this, pLayerTag, L"Boss_Wall_Front_Up")))
    {
        MSG_BOX("Boss Wall Front Up Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_BossWallSide_Up(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CBossWallSideUpper>(this, pLayerTag, L"Boss_Wall_Side_Up")))
    {
        MSG_BOX("Boss Wall Side Up Add Fail");
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CEditScene::Add_Pumpkin(const wstring pLayerTag)
{
    if (FAILED(Add_ObjectToLayer<CPumpkin>(this, pLayerTag, L"Boss_Pumpkin")))
    {
        MSG_BOX("Boss Pumpkin Add Fail");
        return E_FAIL;
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
                if (wcscmp(pObj->m_szDisplayName, L"Terrain") == 0)
                {
                    pVillage = static_cast<CTerrainVillage*>(pObj);
                    break;
                }
            }
        }
    }
}

CGameObject* CEditScene::PickObject(const _vec3& vPickPos)
{
    auto iter = m_umLayer.find(L"Environment_Layer");
    if (iter == m_umLayer.end())
    {
        MSG_BOX("Environment Layer Not Found");
        return nullptr;
    }

    CLayer* pLayer = iter->second;
    const auto& objMap = pLayer->Get_Objects();

    CGameObject* pPicked = nullptr;
    float fBestDist = FLT_MAX;

    const float fPickRadius = 4.0f;

    for (const auto& objListPair : objMap)
    {
        for (CGameObject* pObj : objListPair.second)
        {
            if (pObj == nullptr || wcscmp(pObj->m_szDisplayName, L"Terrain") == 0)
            {
                continue;
            }

            CTransform* pTrans = static_cast<CRenderObject*>(pObj)->Get_Trans();            
            if (pTrans == nullptr)
            {
                continue;
            }

            _vec3 vPos = pTrans->Get_Pos();

            float dx = vPos.x - vPickPos.x;
            float dz = vPos.z - vPickPos.z;
            float distSq = dx * dx + dz * dz;

            if (distSq < fPickRadius && distSq < fBestDist)
            {
                fBestDist = distSq;
                pPicked = pObj;
            }
        }
    }

    return pPicked;
}
