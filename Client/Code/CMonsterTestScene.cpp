#include "pch.h"
#include "CMonsterTestScene.h"

#include "CLayer.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CTreeMob.h"
#include "CSlimeMob.h"
#include "CTerrainVillage.h"

#include "CManagement.h"
#include "CLayerHelper.h"
#include "CDataManager.h"

#include "CCameraManager.h"

CMonsterTestScene::CMonsterTestScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CMonsterTestScene::~CMonsterTestScene()
{
}

HRESULT CMonsterTestScene::Ready_Scene()
{
    if (FAILED(Ready_GameLogic_Layer(CDataManager::GetInstance()->Get_LayerTag(GAMELOGIC_LAYER))))
        return E_FAIL;

    if (FAILED(Ready_Camera_Layer(CDataManager::GetInstance()->Get_LayerTag(CAMERA_LAYER))))
        return E_FAIL;

    return S_OK;
}

_int CMonsterTestScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    CCameraManager::GetInstance()->Update_Camera(fTimeDelta);

    return iExit;
}

void CMonsterTestScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_Camera(fTimeDelta);
}

void CMonsterTestScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CMonsterTestScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    CCameraManager::GetInstance()->Set_CameraMode(CCameraManager::INGAME);
    return S_OK;
}

HRESULT CMonsterTestScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = Get_Layer(wsLayerTag);

    CGameObject* pVillage = nullptr;
    pVillage = CTerrainVillage::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Village", pVillage)))
        return E_FAIL;

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

CMonsterTestScene* CMonsterTestScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonsterTestScene* pLogo = new CMonsterTestScene(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        MSG_BOX("MonsterTestScene Create Failed");
        Safe_Release(pLogo);
        return nullptr;
    }

    return pLogo;
}

void CMonsterTestScene::Free()
{
    CScene::Free();
}
