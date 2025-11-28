#include "pch.h"
#include "CUITestScene.h"
#include "CDInputManager.h"
#include "CManagement.h"
#include "CFontManager.h"
#include "CDataManager.h"

#include "CUIInven.h"
#include "CInvenStatic.h"
#include "CCameraManager.h"
#include "CUIStatic.h"
#include "CLayerHelper.h"
#include "CHpBar.h"



CUITestScene::CUITestScene(LPDIRECT3DDEVICE9 pGraphicDev)
    :CScene(pGraphicDev), m_bCheck(false)
{
}

CUITestScene::~CUITestScene()
{
}

HRESULT CUITestScene::Ready_Scene()
{
    CScene::Ready_Scene();

    if (FAILED(Ready_Environment_Layer(CDataManager::GetInstance()->Get_LayerTag(ENVIRONMENT_LAYER))))
        return E_FAIL;

    if (FAILED(Ready_Camera_Layer(CDataManager::GetInstance()->Get_LayerTag(CAMERA_LAYER))))
        return E_FAIL;

    if (FAILED(Ready_UIInven_Layer(CDataManager::GetInstance()->Get_LayerTag(UI_LAYER))))
        return E_FAIL;

    return S_OK;
}

_int CUITestScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CUITestScene::LateUpdate_Scene(const _float fTimeDelta)
{
    UI_KeyInput(fTimeDelta);

    Engine::CScene::LateUpdate_Scene(fTimeDelta);

}

void CUITestScene::Render_Scene()
{
    
    // 코인 테스트
    _vec2 vPos{ 100.f, 100.f };

    //CFontManager::GetInstance()->Render_Font(L"Font_Moon", L"안녕하세요", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    CFontManager::GetInstance()->Render_Font(L"Font_Default", L"안녕하세요", &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    Engine::CScene::Render_Scene();
}

HRESULT CUITestScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    CCameraManager::GetInstance()->Set_CameraMode(CCameraManager::DBG_PAUSE);

    return S_OK;
}

HRESULT CUITestScene::Ready_Environment_Layer(const wstring& wsLayerTag)
{
    CLayer* CEnviroment = Get_Layer(wsLayerTag);

    return S_OK;
}



HRESULT CUITestScene::Ready_UIInven_Layer(const wstring& wsLayerTag)
{
    CLayer* pLayer = Get_Layer(wsLayerTag);

    if (pLayer == nullptr)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    //인벤 UI Static
    pGameObject = CInvenStatic::Create(m_pGraphicDevice);

    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_InvenStatic", pGameObject)))
        return E_FAIL;

    //인벤 UI
    pGameObject = CUIInven::Create(m_pGraphicDevice);

    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_Inven", pGameObject)))
        return E_FAIL;


    //Static UI
    pGameObject = CUIStatic::Create(m_pGraphicDevice);

    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_Static", pGameObject)))
        return E_FAIL;

    //HpBar
    pGameObject = CHpBar::Create(m_pGraphicDevice);

    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_HpBar", pGameObject)))
        return E_FAIL;

    return S_OK;
}

CUITestScene* CUITestScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUITestScene* CTestScene = new CUITestScene(pGraphicDev);

    if (FAILED(CTestScene->Ready_Scene()))
    {
        Safe_Release(CTestScene);
        MSG_BOX("Stage Create Failed");
        return nullptr;
    }

    return CTestScene;
}

void CUITestScene::UI_KeyInput(const _float& fTimeDelta)
{

    if (CDInputManager::GetInstance()->Get_DIKeyState(DIK_I) & 0x80)
    {

        if (m_bCheck == false)
        {
            CUIInven* pInventory = static_cast<CUIInven*>
                (CManagement::GetInstance()->Get_Object(L"UI_Layer", L"UI_Inven"));

            if (pInventory)
            {
                pInventory->InvenButton();
            }
            m_bCheck = true;

        }

    }
    else
    {
        m_bCheck = false;
    }
}

void CUITestScene::Free()
{
    Engine::CScene::Free();
}
