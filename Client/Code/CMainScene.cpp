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

#include "CPlayerTestScene.h"

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

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CMainScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);    

#pragma region Examples for ImGui

    CExampleManager::GetInstance()->Update_Manager();

#pragma endregion

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_E))
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

#pragma region Test Scene Transition
    if (GetAsyncKeyState('M') & 0x0001)
    {
        CManagement::GetInstance()->Set_Scene(CPlayerTestScene::Create(m_pGraphicDevice));
    }
#pragma endregion


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
    CLayer* pCamLayer = CLayer::Create(wsLayerTag);

   // TODO : ī�޶� ���� ��� ����
   CGameObject* pGameObject = nullptr;
   _vec3 vEye{0.f, 10.f, -10.f}, vAt{0.f, 0.f, 10.f}, vUp{0.f, 1.f, 0.f};
   pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);
   if (FAILED(pCamLayer->Add_GameObject(L"Cam", pGameObject)))
       return E_FAIL;

   m_umLayer.emplace( pair<const wstring&, CLayer*>{ wsLayerTag, pCamLayer} );

    return S_OK;
}

HRESULT CMainScene::Ready_Environment_Layer(const wstring& wsLayerTag)
{
    return S_OK;
}

HRESULT CMainScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = CLayer::Create(wsLayerTag);

    CGameObject* pGameObject = nullptr;
    pGameObject = CTestRect::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Temp", pGameObject)))
        return E_FAIL;
    
#pragma region Examples for ImGui
    pGameObject = CExampleObject::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Example", pGameObject)))
        return E_FAIL;

    CExampleManager::GetInstance()->Ready_Manager();

#pragma endregion


    m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pGameLogicLayer});

    return S_OK;
}

HRESULT CMainScene::Ready_UI_Layer(const wstring& wsLayerTag)
{
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
