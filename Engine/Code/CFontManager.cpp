#include "CFontManager.h"

IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	Free();
}

HRESULT CFontManager::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wsFontTag, const wstring& wsFontType, _uint iWidth, _uint iHeight, _uint iWeight)
{
	CFont* pMyFont = Find_Font(wsFontTag);

    if (nullptr != pMyFont)
    return E_FAIL;

    pMyFont = CFont::Create(pGraphicDev, wsFontType, iWidth, iHeight, iWeight);

    if (nullptr == pMyFont) return E_FAIL;

    m_mapFont.insert({ wsFontTag, pMyFont });

    return S_OK;
    

    /*
    if (Find_Font(wsFontTag))
        return E_FAIL;

    //1. 외부 TTF 파일 읽기 + AddFontMemResourceEx 윈도우에 임시 폰트 등록
    HANDLE hFile = CreateFile(
        L"../Bin/Resource/Font/JejuGothic.ttf",
        GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return E_FAIL;

    DWORD dwSize = GetFileSize(hFile, NULL);
    BYTE* pFontData = new BYTE[dwSize];

    DWORD dwRead = 0;
    ReadFile(hFile, pFontData, dwSize, &dwRead, NULL);
    CloseHandle(hFile);

    // 메모리 등록
    DWORD nFonts = 0;
    AddFontMemResourceEx(pFontData, dwSize, NULL, &nFonts);

    CFont* pMoonFont = CFont::Create(pGraphicDev, wsFontType, iWidth, iHeight, iWeight);

	if (nullptr == pMoonFont)
		return E_FAIL;

	m_mapFont.insert({ wsFontTag, pMoonFont });

	return S_OK;
    */

}

void CFontManager::Render_Font(const wstring& wsFontTag, const wstring& wsString, const _vec2* pPos, D3DXCOLOR Color)
{
	CFont* pFont = Find_Font(wsFontTag);

	pFont->Render_Font(wsString, pPos, Color);
}

CFont* CFontManager::Find_Font(const wstring& wsFontTag)
{
    auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(),
        [&wsFontTag](pair<const wstring, CFont*>& pair) -> _bool {
            return pair.first == wsFontTag;
        });

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontManager::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}
