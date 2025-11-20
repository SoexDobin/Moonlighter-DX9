#include "CTexture.h"
#include "CResourceManager.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs)
{
    size_t iTex = rhs.m_vecTexture.size();

    m_vecTexture.reserve(iTex);
    m_vecTexture = rhs.m_vecTexture;
    for (_uint i = 0; i < iTex; ++i)
    {
        size_t iFrame = rhs.m_vecTexture[i].size();
        m_vecTexture[i] = rhs.m_vecTexture[i];

        for (_uint j = 0; j < iFrame; ++j)
        {
            m_vecTexture[i][j]->AddRef();
        }
    }
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(const wstring wsKey, const _uint iCnt)
{
    m_vecTexture.push_back(CResourceManager::GetInstance()->Get_Sprite(wsKey));

    return S_OK;
}

void CTexture::Set_Texture(const _uint iTex, const _uint iFrame)
{
    if (m_vecTexture.empty() || m_vecTexture.size() <= iTex)
        return;
    if (m_vecTexture[iTex].empty() || m_vecTexture[iTex].size() <= iFrame)
        return;

    m_pGraphicDevice->SetTexture(0, m_vecTexture[iTex][iFrame]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
                           const wstring wsKey, 
                           const _uint iCnt)
{
    CTexture* pInstance = new CTexture(pGraphicDev);

    if (FAILED(pInstance->Ready_Texture(wsKey, iCnt)))
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

    for_each(m_vecTexture.begin(), m_vecTexture.end(), 
        [](vector<IDirect3DBaseTexture9*>& vec) -> void {
            for_each(vec.begin(), vec.end(), CDeleteObj());
    });

    m_vecTexture.clear();
}
