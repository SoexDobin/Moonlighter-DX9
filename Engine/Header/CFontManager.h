#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CFont.h"

BEGIN(Engine)

class ENGINE_DLL CFontManager : public CBase
{
	DECLARE_SINGLETON(CFontManager)

private:
	explicit CFontManager();
	virtual ~CFontManager();

public:
	HRESULT				Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
        const wstring& wsFontTag,
        const wstring& wsFontType,
		_uint iWidth,
		_uint iHeight,
		_uint iWeight);

	void				Render_Font(const wstring& wsFontTag,
        const wstring& wsString,
		const _vec2* pPos,
		D3DXCOLOR Color);

private:
	CFont* Find_Font(const wstring& pFontTag);

private:
	map<wstring, CFont*>			m_mapFont;

private:
	virtual void	Free();
};

END
