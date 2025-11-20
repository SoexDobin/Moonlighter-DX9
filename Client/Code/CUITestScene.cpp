#include "pch.h"
#include "CUITestScene.h"
#include "CUIInven.h"
#include "CDynamicCamera.h"
#include "CPlayer.h"

CUITestScene::CUITestScene(LPDIRECT3DDEVICE9 pGraphicDev)
    :CScene(pGraphicDev)
{
}

CUITestScene::~CUITestScene()
{
}

HRESULT CUITestScene::Ready_Scene()
{ 

    if (FAILED(Ready_Camera_Layer(L"Camera_Layer")))
        return E_FAIL;

    //if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
    //    return E_FAIL;

    if (FAILED(Ready_UIInven_Layer(L"UI_Layer")))
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
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CUITestScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CUITestScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    CLayer* pCamLayer = CLayer::Create(wsLayerTag);
    
    CGameObject* pGameObject = nullptr;
    _vec3 vEye{ 0.f, 10.f, -10.f }, vAt{ 0.f, 0.f, 10.f }, vUp{ 0.f, 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);
    if (FAILED(pCamLayer->Add_GameObject(L"Cam", pGameObject)))
        return E_FAIL;
    
    m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pCamLayer});

    return S_OK;
}


HRESULT CUITestScene::Ready_UIInven_Layer(const wstring& wsLayerTag)
{
    CLayer* pLayer = CLayer::Create(wsLayerTag);

    if (pLayer == nullptr)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    //인벤 UI
    pGameObject = CUIInven::Create(m_pGraphicDevice);

    if (pGameObject == nullptr)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"UI_Invnen", pGameObject)))
        return E_FAIL;


    m_umLayer.emplace(pair<const wstring&, CLayer*>{ wsLayerTag, pLayer});

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

void CUITestScene::Free()
{
    Engine::CScene::Free();
}
