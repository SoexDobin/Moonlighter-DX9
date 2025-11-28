#include "pch.h"
#include "CBossWallFrontUpper.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

CBossWallFrontUpper::CBossWallFrontUpper(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr)
{
}

CBossWallFrontUpper::CBossWallFrontUpper(const CBossWallFrontUpper& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr)
{
}

CBossWallFrontUpper::~CBossWallFrontUpper()
{
}

HRESULT CBossWallFrontUpper::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTextureCom = static_cast<CTexture*>(pCom))
    {
        m_pTextureCom->Ready_Texture(L"Map_Boss_Wall_Up");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pTextureCom));
    }
    return S_OK;

    m_iObjectID = OBJECT_ID::ENVIRONMENT;

}

_int CBossWallFrontUpper::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CBossWallFrontUpper::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CBossWallFrontUpper::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pTextureCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CBossWallFrontUpper* CBossWallFrontUpper::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossWallFrontUpper* pInstance = new CBossWallFrontUpper(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CBossWallFrontUpper Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CBossWallFrontUpper::Free()
{
    Engine::CGameObject::Free();
}
