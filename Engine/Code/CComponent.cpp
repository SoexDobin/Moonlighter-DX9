#include "CComponent.h"
#include "CGameObject.h"
#include "CRenderObject.h"

CComponent::CComponent()
    : m_pGraphicDevice(nullptr), m_bClone(false), m_bDisplayInEditor(false),
    m_pOwner(nullptr), m_pTrans(nullptr), m_bEnable(TRUE), m_bIsDetroy(FALSE)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDevice(pGraphicDev), m_bClone(false), m_bDisplayInEditor(false),
    m_pOwner(nullptr), m_pTrans(nullptr), m_bEnable(TRUE), m_bIsDetroy(FALSE)
{
    m_pGraphicDevice->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
    : m_pGraphicDevice(rhs.m_pGraphicDevice), m_bClone(true), m_bDisplayInEditor(false),
    m_pOwner(nullptr), m_pTrans(nullptr), m_bEnable(TRUE), m_bIsDetroy(FALSE)
{
    m_pGraphicDevice->AddRef();
}

CComponent::~CComponent()
{
    
}

void CComponent::Set_Owner(CGameObject* pOwner)
{
    if (pOwner == nullptr) return;
    m_pOwner = pOwner;

    if (m_pOwner->Get_Type() == RENDER_OBJECT)
        m_pTrans = static_cast<CRenderObject*>(m_pOwner)->Get_Trans();
}

void CComponent::Free()
{
    Safe_Release(m_pGraphicDevice);
    m_pOwner = nullptr;
    m_pTrans = nullptr;
}
