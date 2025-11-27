#include "pch.h"
#include "CTerrainDungeonNormal.h"
#include "CPrototypeManager.h"
#include "CRenderer.h"

CTerrainDungeonNormal::CTerrainDungeonNormal(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr)
{
    PANEL_NAME(L"Terrain")
}

CTerrainDungeonNormal::CTerrainDungeonNormal(const CTerrainDungeonNormal& rhs)
    :CRenderObject(rhs), m_pBufferCom(nullptr), m_pTextureCom(nullptr)
{
    PANEL_NAME(L"Terrain")
}

CTerrainDungeonNormal::~CTerrainDungeonNormal()
{
}

HRESULT CTerrainDungeonNormal::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;
    if (FAILED(Add_Component()))
        return E_FAIL;


    return S_OK;
}

_int CTerrainDungeonNormal::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CTerrainDungeonNormal::LateUpdate_GameObject(const _float fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrainDungeonNormal::Render_GameObject()
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

HRESULT CTerrainDungeonNormal::Add_Component()
{
    Engine::CComponent* pComponent = nullptr;

    // buffer
    pComponent = CPrototypeManager::GetInstance()->Clone_Prototype(TERRAINDUNGEONTEX);
    if (nullptr == pComponent)
    {
        MSG_BOX("Village buffer Component Error");
        return E_FAIL;
    }
    if (pComponent->Get_ComponentType() == TERRAINDUNGEONTEX)
    {
        m_pBufferCom = static_cast<CTerrainDungeonTex*>(pComponent);
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
        m_pTextureCom->Ready_Texture(L"Map_Dungeon");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Com_TerrainTexture", m_pTextureCom));
    }

    return S_OK;
}

CTerrainDungeonNormal* CTerrainDungeonNormal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTerrainDungeonNormal* pTerrain = new CTerrainDungeonNormal(pGraphicDev);

    if (FAILED(pTerrain->Ready_GameObject()))
    {
        MSG_BOX("pTerrain Create Failed");
        Safe_Release(pTerrain);
        return nullptr;
    }

    return pTerrain;
}

void CTerrainDungeonNormal::Free()
{
    CGameObject::Free();
}
