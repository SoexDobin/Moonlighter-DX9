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
	HRESULT				Ready_Texture(PROTOTYPE_COMPONENT eComType, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1);
	void				Set_Texture(const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>	m_vecTexture;
	PROTOTYPE_COMPONENT				m_eComType;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, PROTOTYPE_COMPONENT eComType, TEXTUREID eType, const _tchar* pPath, const _uint& iCnt = 1);
	CComponent*				Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return m_eComType; }

private:
	virtual void		Free() override;
};

END