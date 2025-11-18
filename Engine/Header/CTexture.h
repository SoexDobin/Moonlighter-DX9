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
	HRESULT				Ready_Texture(const wstring wsPath, const _uint iCnt = 1);
	void				Set_Texture(const _uint iTex = 0, const _uint iFrame = 0);

private:
	vector<vector<IDirect3DBaseTexture9*>>	m_vecTexture;
	// 루프
	// 프레임 체크
	// 애니메이션 한바퀴

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring wsKey, const _uint iCnt = 1);
	CComponent*				Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return TEXTURE; }

private:
	virtual void		Free() override;
};

END