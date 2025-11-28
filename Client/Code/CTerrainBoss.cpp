#include "pch.h"
#include "CTerrainBoss.h"
#include "CPrototypeManager.h"
#include "CRenderer.h"

CTerrainBoss::CTerrainBoss(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr)
{
}

CTerrainBoss::CTerrainBoss(const CTerrainBoss& rhs)
    :CRenderObject(rhs), m_pBufferCom(nullptr), m_pTextureCom(nullptr)
{
}

CTerrainBoss::~CTerrainBoss()
{
}

HRESULT CTerrainBoss::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;
    if (FAILED(Add_Component()))
        return E_FAIL;


    return S_OK;
}

_int CTerrainBoss::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CTerrainBoss::LateUpdate_GameObject(const _float fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrainBoss::Render_GameObject()
{

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    if (m_pTransformCom != nullptr)
    {
        m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    }

    if (m_pTextureCom != nullptr)
    {
        m_pTextureCom->Set_Texture(0);
    }

    if (m_pBufferCom != nullptr)
    {
        m_pBufferCom->Render_Buffer();
    }

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CTerrainBoss::Add_Component()
{
    Engine::CComponent* pComponent = nullptr;

    // buffer
    pComponent = CPrototypeManager::GetInstance()->Clone_Prototype(TERRAINBOSSTEX);
    if (nullptr == pComponent)
    {
        MSG_BOX("Village buffer Component Error");
        return E_FAIL;
    }
    if (pComponent->Get_ComponentType() == TERRAINBOSSTEX)
    {
        m_pBufferCom = static_cast<CTerrainBossTex*>(pComponent);
    }

    m_umComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    // texture
    pComponent = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (nullptr == pComponent)
    {
        MSG_BOX("Dungeon texture Component Error");
    }
    if (pComponent->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTextureCom = static_cast<CTexture*>(pComponent))
    {
        m_pTextureCom->Ready_Texture(L"Map_Boss");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Com_TerrainTexture", m_pTextureCom));
    }

    return S_OK;
}

CTerrainBoss* CTerrainBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTerrainBoss* pTerrain = new CTerrainBoss(pGraphicDev);

    if (FAILED(pTerrain->Ready_GameObject()))
    {
        MSG_BOX("pTerrain Create Failed");
        Safe_Release(pTerrain);
        return nullptr;
    }

    return pTerrain;
}

void CTerrainBoss::Free()
{
    CGameObject::Free();
}
