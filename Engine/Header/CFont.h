#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFont :  public CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFont();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXSPRITE				m_pSprite;
	LPD3DXFONT					m_pFont;

public:
	HRESULT			Ready_Font(const wstring& wsFontType,
								_uint iWidth,
								_uint iHeight,
								_uint iWeight);

	void			Render_Font(const wstring& pString, 
								const _vec2* pPos,
								D3DXCOLOR Color);


public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev,
						const wstring& pFontType, 
						_uint iWidth, 
						_uint iHeight, 
						_uint iWeight);

private:
	virtual void		Free();

};

END
