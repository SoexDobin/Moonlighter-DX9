#include "CComponent.h"

CComponent::CComponent()
    : m_pGraphicDevice(nullptr), m_bClone(false), m_bDisplayInEditor(false)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDevice(pGraphicDev), m_bClone(false), m_bDisplayInEditor(false)
{
    m_pGraphicDevice->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
    : m_pGraphicDevice(rhs.m_pGraphicDevice), m_bClone(true), m_bDisplayInEditor(false)
{
    m_pGraphicDevice->AddRef();
}

CComponent::~CComponent()
{
    
}

void CComponent::Free()
{
    Safe_Release(m_pGraphicDevice);
}