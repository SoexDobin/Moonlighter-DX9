#include "CLightManager.h"

IMPLEMENT_SINGLETON(CLightManager)

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
    Free();
}

HRESULT CLightManager::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, _uint iIndex)
{
    CLight* pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
    if (nullptr == pLight)
        return E_FAIL;

    m_LightList.push_back(pLight);

    return S_OK;
}

void CLightManager::Free()
{
    for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
    m_LightList.clear();
}
