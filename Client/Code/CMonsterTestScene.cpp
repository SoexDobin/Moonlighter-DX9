#include "pch.h"
#include "CMonsterTestScene.h"

#include "CLayer.h"
#include "CDynamicCamera.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CTreeMob.h"

#include "CManagement.h"


CMonsterTestScene::CMonsterTestScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{
}

CMonsterTestScene::~CMonsterTestScene()
{
}

HRESULT CMonsterTestScene::Ready_Scene()
{
    if (FAILED(Ready_Camera_Layer(L"Camera_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
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
    CLayer* pCamLayer = CLayer::Create(wsLayerTag);

    CGameObject* pGameObject = nullptr;
    _vec3 vEye{ 0.f, 0.f, -20.f }, vAt{ 0.f, 0.f, 10.f }, vUp{ 0.f, 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDevice, &vEye, &vAt, &vUp);
    if (FAILED(pCamLayer->Add_GameObject(L"Cam", pGameObject)))
        return E_FAIL;

    m_umLayer.emplace(pair<const wstring, CLayer*>{ wsLayerTag, pCamLayer});

    return S_OK;
}

HRESULT CMonsterTestScene::Ready_GameLogic_Layer(const wstring& wsLayerTag)
{
    CLayer* pGameLogicLayer = CLayer::Create(wsLayerTag);

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

    m_umLayer.emplace(pair<const wstring, CLayer*>{ wsLayerTag, pGameLogicLayer});

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
