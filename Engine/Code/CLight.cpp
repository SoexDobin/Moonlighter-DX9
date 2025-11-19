#include "CLight.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDevice(pGraphicDev), m_iIndex(0)
{
    ZeroMemory(&m_tLight, sizeof(D3DLIGHT9));
    m_pGraphicDevice->AddRef();
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo, _uint iIndex)
{
    memcpy(&m_tLight, pLightInfo, sizeof(D3DLIGHT9));

    m_iIndex = iIndex;

    m_pGraphicDevice->SetLight(iIndex, pLightInfo);
    m_pGraphicDevice->LightEnable(iIndex, TRUE);

    return S_OK;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, _uint iIndex)
{
    CLight* pLight = new CLight(pGraphicDev);

    if (FAILED(pLight->Ready_Light(pLightInfo, iIndex)))
    {
        Safe_Release(pLight);
        MSG_BOX("Light Create Failed");
        return nullptr;
    }

    return pLight;
}

void CLight::Free()
{
    m_pGraphicDevice->LightEnable(m_iIndex, FALSE);

    Safe_Release(m_pGraphicDevice);
}
