#include "pch.h"
#include "CPumpkin.h"
#include "CRenderer.h"
#include "CPrototypeManager.h"
#include "CDInputManager.h"
#include "CManagement.h"
#include "CRectCollider.h"

CPumpkin::CPumpkin(LPDIRECT3DDEVICE9 pGraphicDev)
    : CRenderObject(pGraphicDev), m_pTextureCom(nullptr)
{
}

CPumpkin::CPumpkin(const CPumpkin& rhs)
    : CRenderObject(rhs), m_pTextureCom(nullptr)
{
}

CPumpkin::~CPumpkin()
{
}

HRESULT CPumpkin::Ready_GameObject()
{
    if (FAILED(Engine::CRenderObject::Ready_GameObject()))
        return E_FAIL;

    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    if (m_pTextureCom = static_cast<CTexture*>(pCom))
    {
        m_pTextureCom->Ready_Texture(L"Map_Boss_Pumpkin");

        m_pTextureCom->Set_Texture(0);

        m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"Texture_Com", m_pTextureCom));
    }

    if (m_pRectCollider = Add_Component<CRectCollider>(ID_DYNAMIC, L"Pumpkin_Collider", RECT_COLLIDER))
    {
        //m_pRectCollider->Set_Dimension({4.f, 4.f, 1.f});
        //m_pRectCollider->Set_Offset({10.f, 10.f, 3.f});
        //m_pRectCollider->Set_Scale();

        //m_pRectCollider->Get_ColType() == RECT_COL
    }

    m_pTransformCom->Set_Scale(4.f, 4.f, 0.f);

    return S_OK;
}

_int CPumpkin::Update_GameObject(const _float fTimeDelta)
{
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    if (CDInputManager::GetInstance()->Get_DIKeyPressed(DIK_0))
        Destroy(this);

    return iExit;
}

void CPumpkin::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CPumpkin::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    m_pTextureCom->SetUp_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CPumpkin* CPumpkin::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPumpkin* pInstance = new CPumpkin(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        MSG_BOX("CPumpkin Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CPumpkin::Free()
{
    Engine::CGameObject::Free();
}
