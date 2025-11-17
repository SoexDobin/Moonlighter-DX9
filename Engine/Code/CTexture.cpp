#include "CTexture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), m_eComType(TEXTURE_TERRAIN)
{
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs), m_eComType(rhs.m_eComType)
{
    size_t iSize = rhs.m_vecTexture.size();

    m_vecTexture.reserve(iSize);
    m_vecTexture = rhs.m_vecTexture;
    
    for (_uint i = 0; i < iSize; ++i)
        m_vecTexture[i]->AddRef();
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(PROTOTYPE_COMPONENT eComType, TEXTUREID eType, const wstring wsPath, const _uint iCnt)
{
    if (eComType >= COMPONENT_END || eComType < 0)
    {
        MSG_BOX("Wrong Component Type at CTexture.cpp");
        return E_FAIL;
    }
        
    m_eComType = eComType;
    m_vecTexture.reserve(iCnt);

    IDirect3DBaseTexture9* pTexture = nullptr;

    for (_uint i = 0; i < iCnt; ++i)
    {
        TCHAR   szFileName[256] = L"";
        wsprintf(szFileName, wsPath.c_str(), i);

        switch (eType)
        {
        case Engine::TEX_NORMAL:
            if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDevice, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
                return E_FAIL;
            break;

        case Engine::TEX_CUBE:
            if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDevice, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
                return E_FAIL;
            break;

        default: 
            continue;
        }

        m_vecTexture.push_back(pTexture);
    }

    return S_OK;
}

void CTexture::Set_Texture(const _uint& iIndex)
{
    if (m_vecTexture.empty() || m_vecTexture.size() <= iIndex)
        return;

    m_pGraphicDevice->SetTexture(0, m_vecTexture[iIndex]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
                           PROTOTYPE_COMPONENT eComType,
                           TEXTUREID eType, 
                           const wstring wsPath, 
                           const _uint iCnt)
{
    CTexture* pInstance = new CTexture(pGraphicDev);

    if (FAILED(pInstance->Ready_Texture(eComType, eType, wsPath, iCnt)))
    {
        MSG_BOX("Texture Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CComponent* CTexture::Clone()
{
    return new CTexture(*this);
}

void CTexture::Free()
{
    Engine::CComponent::Free();

    for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
    m_vecTexture.clear();
}
