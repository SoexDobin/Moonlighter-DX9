#include "pch.h"
#include "CMainScene.h"
#include "CDInputManager.h"
#include "CPrototypeManager.h"
#include "CDynamicCamera.h"
#include "CTestRect.h"
#include "CExampleObject.h"
#include "CExampleManager.h"
#include "CLightManager.h"
#include "CManagement.h"
#include "CEditScene.h"

#include "CUIInven.h"
#include "CUIStatic.h"

#include "CPlayer.h"
#include "CBoss.h"
#include "CSlimeMob.h"
#include "CTreeMob.h"

#include "CPlayerTestScene.h"
#include "CUtility.h"

CMainScene::CMainScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Ready_Scene()
{
    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Camera_Layer(L"Camera_Layer")))
        return E_FAIL;
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;
    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;
    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CMainScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

#pragma region Examples for ImGui

    CExampleManager::GetInstance()->Update_Manager();

#pragma endregion

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_M))
    {
        Engine::CScene* pEdit = CEditScene::Create(m_pGraphicDevice);

        if (nullptr == pEdit)
            return -1;

        if (FAILED(CManagement::GetInstance()->Set_Scene(pEdit)))
        {
            MSG_BOX("Stage Setting Failed");
            return -1;
        }
    }
    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_I) & 0x80)
    {

        CUIInven* pInventory = static_cast<CUIInven*>
            (CManagement::GetInstance()->Get_Object(L"UI_Layer", L"UI_Invnen"));

        if (pInventory)
        {
            pInventory->InvenButton();
        }
    }
    return iExit;
}

void CMainScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CMainScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CMainScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    //CLayer* pCamLayer = CLayer::Create(wsLayerTag);

    //CGameObject* pGameObject = nullptr;
    //_vec3 vEye{ 0.f, 5.f, -20.f }, vAt{ 0.f, 0.f, 10.f }, vUp{ 0.f, 1.f, 0.f };
    //pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);
    //if (FAILED(pCamLayer->Add_GameObject(L"Cam", pGameObject)))
    //    return E_FAIL;

    //m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pCamLayer});


    return S_OK;
}

HRESULT CMainScene::Ready_Environment_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = CLayer::Create(wsLayerTag);

    CGameObject* pGameObject = nullptr;
    pGameObject = CTerrainVillage::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Village", pGameObject)))
        return E_FAIL;

    m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pGameLogicLayer});
    return S_OK;
}

HRESULT CMainScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = CLayer::Create(wsLayerTag);

    CGameObject* pGameObject = nullptr;
    pGameObject = CTestRect::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Temp", pGameObject)))
        return E_FAIL;

    pGameObject = CTestRect::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Temp2", pGameObject)))
        return E_FAIL;

    pGameObject = CTestRect::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Temp3", pGameObject)))
        return E_FAIL;

    // fix
    //pGameObject = CTerrainVillage::Create(m_pGraphicDevice);
    //if (FAILED(pGameLogicLayer->Add_GameObject(L"Vill", pGameObject)))
    //    return E_FAIL;

    CGameObject* pPlayer = nullptr;
    pPlayer = CPlayer::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Player", pPlayer)))
        return E_FAIL;

    CGameObject* pBoss = nullptr;
    pBoss = CBoss::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Boss", pBoss)))
        return E_FAIL;

    CGameObject* pTreeMob = nullptr;
    pTreeMob = CTreeMob::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"TreeMob", pTreeMob)))
        return E_FAIL;

    CGameObject* pSlimeMob = nullptr;
    pSlimeMob = CSlimeMob::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"SlimeMob", pSlimeMob)))
        return E_FAIL;

#pragma region Examples for ImGui
    //pGameObject = CExampleObject::Create(m_pGraphicDevice);
    //if (FAILED(pGameLogicLayer->Add_GameObject(L"Example", pGameObject)))
    //    return E_FAIL;

    //CExampleManager::GetInstance()->Ready_Manager();

#pragma endregion

    m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pGameLogicLayer});

    return S_OK;
}

HRESULT CMainScene::Ready_UI_Layer(const wstring& wsLayerTag)
{
    CLayer* pLayer = CLayer::Create(wsLayerTag);

    Engine::CGameObject* pGameObject = nullptr;

    //인벤 UI
    pGameObject = CUIInven::Create(m_pGraphicDevice);
    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_Invnen", pGameObject)))
        return E_FAIL;

    //Static UI
    pGameObject = CUIStatic::Create(m_pGraphicDevice);
    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_Static", pGameObject)))
        return E_FAIL;

    m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pLayer});

    return S_OK;
}

HRESULT CMainScene::Ready_Light()
{

    D3DLIGHT9   tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

    tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    tLightInfo.Direction = { 1.f, -1.f, 1.f };

    if (FAILED(CLightManager::GetInstance()->Ready_Light(m_pGraphicDevice, &tLightInfo, 0)))
        return E_FAIL;


    return S_OK;
}

HRESULT CMainScene::Ready_Prototype()
{
    return S_OK;
}

CMainScene* CMainScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMainScene* pLogo = new CMainScene(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("MainScene Create Failed");
        Safe_Release(pLogo);
        return nullptr;
    }

    return pLogo;
}

void CMainScene::Free()
{
#pragma region Examples for ImGui

    CExampleManager::DestroyInstance();

#pragma endregion

    Engine::CScene::Free();
}
