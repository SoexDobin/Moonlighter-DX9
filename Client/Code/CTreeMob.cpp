#include "pch.h"
#include "CTreeMob.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

#include "CPlayer.h"

CTreeMob::CTreeMob(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pDynamicTexCom(nullptr)
{
    PANEL_NAME(L"Tree_Mob");
}

CTreeMob::CTreeMob(const CTreeMob& rhs)
    : CRenderObject(rhs), m_pDynamicTexCom(nullptr)
{
    PANEL_NAME(L"Tree_Mob");
}

CTreeMob::~CTreeMob()
{
    PANEL_NAME(L"Tree_Mob");
}

HRESULT CTreeMob::Ready_GameObject()
{
    // Texture, Transform 컴포넌트 생성 
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pDynamicTexCom = static_cast<CTexture*>(pCom))
    {
        m_pDynamicTexCom->Set_Speed(10.f);
        m_pDynamicTexCom->Ready_Texture(L"TEST");

        m_pDynamicTexCom->Set_Texture(TREE_STATE::T_IDLE);

        m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pDynamicTexCom));
    }


    // after all components are set up
    Configure_Component();

    return S_OK;
}

_int CTreeMob::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CTreeMob::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CTreeMob::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTreeMob::Configure_Component()
{
    m_pTransformCom->Set_Scale(10.f, 10.f, 1.f);
    m_pTransformCom->Set_Pos(0.f, 5.f, -15.f);
}

void CTreeMob::Find_Player()
{
}

void CTreeMob::Chase_Player()
{
    if (nullptr == pPlayer)
    {

    }
}

CTreeMob* CTreeMob::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTreeMob* pInstance = new CTreeMob(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CTreeMob Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CTreeMob::Free()
{
    Engine::CGameObject::Free();
}
