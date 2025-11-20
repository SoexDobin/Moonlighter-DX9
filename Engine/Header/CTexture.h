#pragma once

#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev, _float fSpeed);
	explicit CTexture(const CTexture& rhs);
	virtual	~CTexture() override;

public:
	const _uint			Get_FrameCount(const _uint iTex);

	_bool			Get_Loop()						{ return m_bLoop; }
	void			Set_Loop(_bool bIsLoop)			{ m_bLoop = bIsLoop; }
	_bool			Get_OneLoop()					{ return m_bOneLoop; }
	void			Set_OneLoop(_bool bIsOneLoop)	{ m_bOneLoop = bIsOneLoop; }

	_int			Get_CurTex()					{ return m_iCurTex; }
	_int			Get_CurFrame()					{ return m_iCurFrame; }
	_float			Get_Speed()						{ return m_fSpeed; }
	void			Set_Speed(_float fSpeed)		{ m_fSpeed = fSpeed; }

public:
	void			Stop_Anim() { m_bStop = true; }
	void			Resume_Anim() { m_bStop = false; }


public:
	HRESULT				Ready_Texture(const wstring& wsPath);
	void				Set_Texture(const _uint iTex, const _uint iFrame = 0);

	_int				Update_Component(const _float fTimeDelta) override;
	void				LateUpdate_Component() override;

private:
	vector<vector<IDirect3DBaseTexture9*>>	m_vecTexture;
	_bool	m_bStop;
	_bool	m_bLoop;
	_bool	m_bOneLoop;

	_uint	m_iCurTex;
	_uint	m_iCurFrame;

	_float	m_fSpeed;
	_float  m_fFrameAcc;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fSpeed);
	CComponent*				Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return TEXTURE; }

private:
	virtual void		Free() override;

#pragma region Editor
public:
	virtual void Display_Editor(const char* pObjTag) override;
#pragma endregion

};

END
