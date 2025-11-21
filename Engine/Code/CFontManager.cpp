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

	if (nullptr == pMyFont)
		return E_FAIL;

	m_mapFont.insert({ wsFontTag, pMyFont });

	return S_OK;
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
