#include "CResourceManager.h"

IMPLEMENT_SINGLETON(CResourceManager)

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
    Free();
}

HRESULT CResourceManager::Add_Sprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring wsKey, const wstring wsPath, _uint iCnt)
{
    m_umapSprite.insert({ wsKey, vector<IDirect3DBaseTexture9*>()});
    m_umapSprite[wsKey].reserve(iCnt);
    IDirect3DBaseTexture9* pTexture = nullptr;

    for (_uint i = 0; i < iCnt; ++i)
    {
        TCHAR   szFileName[256] = L"";
        wsprintf(szFileName, wsPath.c_str(), i);

        if (FAILED(D3DXCreateTextureFromFile(pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
            return E_FAIL;

        m_umapSprite[wsKey].push_back(pTexture);
    }

    return S_OK;
}

HRESULT CResourceManager::Add_Cube(LPDIRECT3DDEVICE9 pGraphicDev, const wstring swKey, const wstring swPath)
{
    return S_OK;
}

const vector<IDirect3DBaseTexture9*>& CResourceManager::Get_Sprite(const wstring swKey)
{
    for (_uint i = 0; i < m_umapSprite[swKey].size(); ++i)
        m_umapSprite[swKey][i]->AddRef();

    return m_umapSprite[swKey];
}

void CResourceManager::Free()
{
    for_each(m_umapSprite.begin(), m_umapSprite.end(), 
        [](pair<const wstring, vector<IDirect3DBaseTexture9*>>& pair) -> void {
        for_each(pair.second.begin(), pair.second.end(), CDeleteObj());
        });

    for_each(m_umapCube.begin(), m_umapCube.end(), 
        [](pair<const wstring, vector<IDirect3DCubeTexture9*>>& pair) -> void {
        for_each(pair.second.begin(), pair.second.end(), CDeleteObj());
        });
}
