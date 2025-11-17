#include "pch.h"
#include "CMainScene.h"

CMainScene::CMainScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CMainScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

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

HRESULT CMainScene::Ready_Camera_Layer(const _tchar* pLayerTag)
{
    return S_OK;
}

HRESULT CMainScene::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    return S_OK;
}

HRESULT CMainScene::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
    return S_OK;
}

HRESULT CMainScene::Ready_UI_Layer(const _tchar* pLayerTag)
{
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
    Engine::CScene::Free();
}
