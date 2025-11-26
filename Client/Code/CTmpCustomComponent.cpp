#include "pch.h"
#include "CTmpCustomComponent.h"

CTmpCustomComponent::CTmpCustomComponent()
{
    strcpy_s(m_szDisplayName, "Cient Custom Component");
}

CTmpCustomComponent::CTmpCustomComponent(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
    strcpy_s(m_szDisplayName, "Cient Custom Component");
}

CTmpCustomComponent::CTmpCustomComponent(const CTmpCustomComponent& rhs)
    : CComponent(rhs)
{
    strcpy_s(m_szDisplayName, "Cient Custom Component");
}

CTmpCustomComponent::~CTmpCustomComponent()
{
}

_int CTmpCustomComponent::Update_Component(const _float fTimeDelta)
{
    return 0;
}

void CTmpCustomComponent::LateUpdate_Component()
{
    int a = 0;
}

CTmpCustomComponent* CTmpCustomComponent::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTmpCustomComponent* pInstance = new CTmpCustomComponent(pGraphicDev);

    if (!pInstance)
        return nullptr;

    return pInstance;
}

CComponent* CTmpCustomComponent::Clone()
{
    return new CTmpCustomComponent(*this);
}

void CTmpCustomComponent::Free()
{
    Engine::CComponent::Free();
}

void CTmpCustomComponent::Display_Editor(const char* pObjTag)
{

    ImGui::Text("Cient Custom Component");
    ImGui::Text("Test Component");
}
