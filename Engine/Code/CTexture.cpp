#include "CTexture.h"
#include "CResourceManager.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), 
    m_bLoop(true), m_bOneLoop(false),
    m_iPrevTex(0), m_iPrevFrame(0)
{
}

CTexture::CTexture(const CTexture& rhs)
    : CComponent(rhs),
    m_bLoop(false), m_bOneLoop(false),
    m_iPrevTex(0), m_iPrevFrame(0)
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

const _uint CTexture::Get_FrameCount(const _uint iTex) { 
    return m_vecTexture[iTex].size(); 
}
_bool   CTexture::Get_Loop()                    { return m_bLoop; }
void    CTexture::Set_Loop(_bool bIsLoop)       { m_bLoop = bIsLoop; }
_bool   CTexture::Get_OneLoop()                 { return m_bOneLoop; }
void    CTexture::Set_OneLoop(_bool bIsOneLoop) { m_bOneLoop = bIsOneLoop; }

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

    if (m_vecTexture[iTex].size() == iFrame - 1) 
        m_bOneLoop = true;

    if (m_bOneLoop && !m_bLoop) 
        return;

    m_pGraphicDevice->SetTexture(0, m_vecTexture[iTex][iFrame]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTexture* pInstance(nullptr);

    if (pInstance = new CTexture(pGraphicDev))
    {
        return pInstance;       
    }

    MSG_BOX("Texture Create Failed");
    Safe_Release(pInstance);
    return nullptr;
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
