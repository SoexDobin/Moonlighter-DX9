#include "pch.h"
#include "CComposeScene.h"
#include "CDInputManager.h"
#include "CExampleObject.h"
#include "CExampleManager.h"
#include "CLightManager.h"
#include "CManagement.h"
#include "CEditScene.h"
#include "CLayerHelper.h"
#include "CDataManager.h"

#include "CCameraManager.h"
#include "CUIInven.h"
#include "CUIStatic.h"

#include "CPlayer.h"
#include "CBoss.h"
#include "CSlimeMob.h"
#include "CTreeMob.h"

#include "CPlayerTestScene.h"
#include "CUtility.h"

CComposeScene::CComposeScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CComposeScene::~CComposeScene()
{
}

HRESULT CComposeScene::Ready_Scene()
{
    Engine::CScene::Ready_Scene();

    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Prototype()))
        return E_FAIL;


    if (FAILED(Ready_Environment_Layer(CDataManager::GetInstance()->Get_LayerTag(ENVIRONMENT_LAYER))))
        return E_FAIL;
    if (FAILED(Ready_GameLogic_Layer(CDataManager::GetInstance()->Get_LayerTag(GAMELOGIC_LAYER))))
        return E_FAIL;
    if (FAILED(Ready_Camera_Layer(CDataManager::GetInstance()->Get_LayerTag(CAMERA_LAYER))))
        return E_FAIL;
    if (FAILED(Ready_UI_Layer(CDataManager::GetInstance()->Get_LayerTag(UI_LAYER))))
        return E_FAIL;

    return S_OK;
}

_int CComposeScene::Update_Scene(const _float fTimeDelta)
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

    CCameraManager::GetInstance()->Update_Camera(fTimeDelta);

    return iExit;
}

void CComposeScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);

    CCameraManager::GetInstance()->LateUpdate_Camera(fTimeDelta);

    //auto& layers = Get_Layers();
    //auto iter = layers.find(Engine::CLayerHelper::GetInstance()->GetLayerIDByName(L"GameLogic_Layer"));
    //auto iter2 = layers.find(Engine::CLayerHelper::GetInstance()->GetLayerIDByName(L"Environment_Layer"));

    //if (iter != layers.end() && iter2 != layers.end())
    //{
    //    CComponent* pCom = iter->second->Get_Component(ID_DYNAMIC, L"Player", L"Collider_Com");
    //    CComponent* pCom2 = iter2->second->Get_Component(ID_DYNAMIC, L"Map_Trigger", L"Collider_Com");

    //    if (pCom == nullptr || pCom2 == nullptr)
    //    {
    //        return;
    //    }

    //    if (pCom != nullptr)
    //    {
    //        CCollider* pCollider = static_cast<CRectCollider*>(pCom);
    //        CCollider* pCollider2 = static_cast<CRectCollider*>(pCom2);
    //        if (pCollider->Check_Collision(pCollider2))
    //        {
    //            Engine::CScene* pEdit = CEditScene::Create(m_pGraphicDevice);

    //            if (nullptr == pEdit)
    //                return;

    //            if (FAILED(CManagement::GetInstance()->Set_Scene(pEdit)))
    //            {
    //                MSG_BOX("Stage Setting Failed");
    //                return;
    //            }
    //        }
    //    }
    //}
}

void CComposeScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CComposeScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    CCameraManager::GetInstance()->Set_CameraMode(CCameraManager::INGAME);

    return S_OK;
}

HRESULT CComposeScene::Ready_Environment_Layer(const wstring& wsLayerTag)
{
    CUtility::LoadBossMap(m_pGraphicDevice, Get_Layers());

    return S_OK;
}

HRESULT CComposeScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = Get_Layer(wsLayerTag);
    if (pGameLogicLayer == nullptr) return E_FAIL;

    CGameObject* pPlayer = nullptr;
    pPlayer = CPlayer::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Player", pPlayer)))
        return E_FAIL;

    // TODO : 임시로 플레이어 위치 삽입
    CCameraManager::GetInstance()->Set_Target(static_cast<CTransform*>(pPlayer->Get_Component(ID_DYNAMIC, TRANSFORM)));

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

    return S_OK;
}

HRESULT CComposeScene::Ready_UI_Layer(const wstring& wsLayerTag)
{
    CLayer* pLayer = Get_Layer(wsLayerTag);
    if (pLayer == nullptr) return E_FAIL;

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

    return S_OK;
}

HRESULT CComposeScene::Ready_Light()
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

HRESULT CComposeScene::Ready_Prototype()
{
    return S_OK;
}

CComposeScene* CComposeScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CComposeScene* pLogo = new CComposeScene(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("MainScene Create Failed");
        Safe_Release(pLogo);
        return nullptr;
    }

    return pLogo;
}

void CComposeScene::Free()
{
#pragma region Examples for ImGui

    CExampleManager::DestroyInstance();

#pragma endregion

    Engine::CScene::Free();

}
