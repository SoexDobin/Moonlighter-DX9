#pragma once

#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual	~CTexture() override;

public:
	const _uint			Get_FrameCount(const _uint iTex);

	_bool			Get_Loop();
	void			Set_Loop(_bool bIsLoop);
	_bool			Get_OneLoop();
	void			Set_OneLoop(_bool bIsOneLoop);

public:
	HRESULT				Ready_Texture(const wstring wsPath, const _uint iCnt = 1);
	void				Set_Texture(const _uint iTex = 0, const _uint iFrame = 0);

private:
	vector<vector<IDirect3DBaseTexture9*>>	m_vecTexture;
	_bool	m_bLoop;
	_bool	m_bOneLoop;
	_uint	m_iPrevTex;
	_uint	m_iPrevFrame;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*				Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return TEXTURE; }

private:
	virtual void		Free() override;
};

END