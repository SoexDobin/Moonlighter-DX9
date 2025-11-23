#include "pch.h"
#include "CUIInvenSlot.h"
#include "CPrototypeManager.h"

CUIInvenSlot::CUIInvenSlot(LPDIRECT3DDEVICE9 pGraphicDev)
    :CRenderObject(pGraphicDev)
{
}

CUIInvenSlot::CUIInvenSlot(const CUIInvenSlot& rhs)
    :CRenderObject(rhs)
{
}

CUIInvenSlot::~CUIInvenSlot()
{
}

HRESULT CUIInvenSlot::Ready_GameObject()
{
    CComponent* pCom(nullptr);

    pCom = CPrototypeManager::GetInstance()->Clone_Prototype(TEXTURE);
    if (!pCom || pCom->Get_ComponentType() != TEXTURE)
        return E_FAIL;

    m_pTextureCom = static_cast<CTexture*>(pCom);

    m_pTextureCom->Ready_Texture(L"TestSlot");
    m_pTextureCom->Set_Texture(0);


    m_umComponent[ID_STATIC].insert(pair<wstring, CComponent*>(L"TestSlot", m_pTextureCom));

    m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
    m_pTransformCom->Set_Scale(m_fWidth, m_fHeight, 0.f);

    return S_OK;
}

_int CUIInvenSlot::Update_GameObject(const _float fTimeDelta)
{
   
    _int iExit = Engine::CRenderObject::Update_GameObject(fTimeDelta);

    // 렌더 인벤에서 넣을까? 여기서 넣을까?
    return iExit;
}

void CUIInvenSlot::LateUpdate_GameObject(const _float fTimeDelta)
{
    Engine::CRenderObject::LateUpdate_GameObject(fTimeDelta);
}

void CUIInvenSlot::Render_GameObject()
{
    m_pGraphicDevice->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();

}

void CUIInvenSlot::Set_Pos(_float fx, _float fy)
{
    m_pTransformCom->Set_Pos(fx, fy, 0.f);
}

CUIInvenSlot* CUIInvenSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fx, _float fy)
{
    CUIInvenSlot* pSlot = new CUIInvenSlot(pGraphicDev);

    if (FAILED(pSlot->Ready_GameObject()))
    {
        MSG_BOX("UI_Menu Create Failed");
        Safe_Release(pSlot);
        return nullptr;
    }

    return pSlot;


    return nullptr;
}

void CUIInvenSlot::Free()
{
    CRenderObject::Free();
}
