#include "pch.h"
#include "CBossWallSide.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"

CBossWallSide::CBossWallSide(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr)
{
}

CBossWallSide::CBossWallSide(const CBossWallSide& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr)
{
}

CBossWallSide::~CBossWallSide()
{
}

HRESULT CBossWallSide::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTextureCom = static_cast<CTexture*>(pCom))
    {
        m_pTextureCom->Ready_Texture(L"Map_Boss_Wall_Side");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_DYNAMIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pTextureCom));
    }

    m_pTransformCom->Add_Rotation(ROT_Y, 1.57f);

    m_iObjectID = OBJECT_ID::ENVIRONMENT;

    return S_OK;
}

_int CBossWallSide::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CBossWallSide::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CBossWallSide::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pTextureCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CBossWallSide* CBossWallSide::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossWallSide* pInstance = new CBossWallSide(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CBossWallSide Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CBossWallSide::Free()
{
    Engine::CGameObject::Free();
}
