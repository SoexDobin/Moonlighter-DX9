#include "pch.h"
#include "CBoss.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

#include "CPlayer.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pDynamicTexCom(nullptr)
{
    PANEL_NAME(L"Boss");
}

CBoss::CBoss(const CBoss& rhs)
    : CRenderObject(rhs), m_pDynamicTexCom(nullptr)
{
    PANEL_NAME(L"Boss");
}

CBoss::~CBoss()
{
    PANEL_NAME(L"Boss");
}

HRESULT CBoss::Ready_GameObject()
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
        m_pDynamicTexCom->Ready_Texture(L"Boss_Idle");

        m_pDynamicTexCom->Set_Texture(BOSS_STATE::B_IDLE);

        m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pDynamicTexCom));
    }


    // after all components are set up
    Configure_Component();

    return S_OK;
}

_int CBoss::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CBoss::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CBoss::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss::Configure_Component()
{
    m_pTransformCom->Set_Scale(8.f, 8.f, 1.f);
}

void CBoss::Find_Player()
{
}

void CBoss::Chase_Player()
{
    if (nullptr == pPlayer)
    {
        
    }
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoss* pInstance = new CBoss(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CBoss Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CBoss::Free()
{
    Engine::CGameObject::Free();
}
