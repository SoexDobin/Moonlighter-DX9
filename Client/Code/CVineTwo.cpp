#include "pch.h"
#include "CVineTwo.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

CVineTwo::CVineTwo(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr)
{
}

CVineTwo::CVineTwo(const CVineTwo& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr)
{
}

CVineTwo::~CVineTwo()
{
}

HRESULT CVineTwo::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTextureCom = static_cast<CTexture*>(pCom))
    {
        m_pTextureCom->Ready_Texture(L"Map_Boss_Vine2");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pTextureCom));
    }

    m_pTransformCom->Set_Scale(4.f, 4.f, 1.f);

    m_iObjectID = OBJECT_ID::ENVIRONMENT;

    return S_OK;
}

_int CVineTwo::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CVineTwo::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CVineTwo::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pTextureCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CVineTwo* CVineTwo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CVineTwo* pInstance = new CVineTwo(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CVineTwo Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CVineTwo::Free()
{
    Engine::CGameObject::Free();
}
