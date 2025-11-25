#include "pch.h"
#include "CDungeonWall.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

CDungeonWall::CDungeonWall(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr)
{
    PANEL_NAME(L"Dungeon_Wall")
}

CDungeonWall::CDungeonWall(const CDungeonWall& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr)
{
    PANEL_NAME(L"Dungeon_Wall")
}

CDungeonWall::~CDungeonWall()
{
}

HRESULT CDungeonWall::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTextureCom = static_cast<CTexture*>(pCom))
    {
        m_pTextureCom->Ready_Texture(L"Map_Dungeon_Wall");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pTextureCom));
    }

    m_pTransformCom->Set_Scale(4.f, 4.f, 1.f);
    return S_OK;
}

_int CDungeonWall::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CDungeonWall::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CDungeonWall::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pTextureCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CDungeonWall* CDungeonWall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CDungeonWall* pInstance = new CDungeonWall(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CDungeonWall Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CDungeonWall::Free()
{
    Engine::CGameObject::Free();
}
