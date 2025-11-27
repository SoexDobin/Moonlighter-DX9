#include "pch.h"
#include "CMonsterTestScene.h"

#include "CLayer.h"
#include "CDynamicCamera.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CTreeMob.h"
#include "CSlimeMob.h"
#include "CTerrainVillage.h"

#include "CManagement.h"
#include "CLayerHelper.h"
#include "CDataManager.h"


CMonsterTestScene::CMonsterTestScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CMonsterTestScene::~CMonsterTestScene()
{
}

HRESULT CMonsterTestScene::Ready_Scene()
{
    if (FAILED(Ready_Camera_Layer(CDataManager::GetInstance()->Get_LayerTag(1))))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(CDataManager::GetInstance()->Get_LayerTag(5))))
        return E_FAIL;

    return S_OK;
}

_int CMonsterTestScene::Update_Scene(const _float fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    return iExit;
}

void CMonsterTestScene::LateUpdate_Scene(const _float fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CMonsterTestScene::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CMonsterTestScene::Ready_Camera_Layer(const wstring& wsLayerTag)
{
    CLayer* pCamLayer = CLayerHelper::GetInstance()->Get_Layer(wsLayerTag);

    CGameObject* pGameObject = nullptr;
    _vec3 vEye{ 0.f, 0.f, -20.f }, vAt{ 0.f, 0.f, 10.f }, vUp{ 0.f, 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);
    if (FAILED(pCamLayer->Add_GameObject(L"Cam", pGameObject)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonsterTestScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = CLayerHelper::GetInstance()->Get_Layer(wsLayerTag);

    CGameObject* pVillage = nullptr;
    pVillage = CTerrainVillage::Create(m_pGraphicDevice);
    if (FAILED(pGameLogicLayer->Add_GameObject(L"Village", pVillage)))
        return E_FAIL;

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
