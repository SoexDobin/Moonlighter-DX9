#include "pch.h"
#include "CPlayerTestScene.h"

#include "CLayer.h"
#include "CPlayer.h"

#include "CManagement.h"
#include "CEditScene.h"
#include "CLayerHelper.h"

#include "CCameraManager.h"

CPlayerTestScene::CPlayerTestScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CPlayerTestScene::~CPlayerTestScene()
{
}

HRESULT CPlayerTestScene::Ready_Scene()
{
    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_Camera_Layer(L"Camera_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CPlayerTestScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    CCameraManager::GetInstance()->Update_Camera(fTimeDelta);

    return iExit;
}

void CPlayerTestScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);

    CCameraManager::GetInstance()->LateUpdate_Camera(fTimeDelta);

}

void CPlayerTestScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CPlayerTestScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    CCameraManager::GetInstance()->Set_CameraMode(CCameraManager::INGAME);

    return S_OK;
}

HRESULT CPlayerTestScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = Get_Layer(wsLayerTag);

    CGameObject* pPlayer     = nullptr;
    CGameObject* pGameObject = nullptr;

    pPlayer = CPlayer::Create(m_pGraphicDevice);
    pGameObject = CTerrainVillage::Create(m_pGraphicDevice);

    if (FAILED(pGameLogicLayer->Add_GameObject(L"Player", pPlayer)))
        return E_FAIL;

    // TODO : 임시로 플레이어 위치 삽입
    CCameraManager::GetInstance()->Set_Target(static_cast<CTransform*>(pPlayer->Get_Component(ID_DYNAMIC, TRANSFORM)));


    if (FAILED(pGameLogicLayer->Add_GameObject(L"Village", pGameObject)))
        return E_FAIL;

    return S_OK;
}

CPlayerTestScene* CPlayerTestScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayerTestScene* pLogo = new CPlayerTestScene(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("PlayerTestScene Create Failed");
        Safe_Release(pLogo);
        return nullptr;
    }

    return pLogo;
}

void CPlayerTestScene::Free()
{
    CScene::Free();
}
