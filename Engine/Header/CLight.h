#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight() override;

public:
	HRESULT		Ready_Light(const D3DLIGHT9* pLightInfo, _uint iIndex);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDevice;
	D3DLIGHT9				m_tLight;
	_uint					m_iIndex;

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, _uint iIndex);
private:
	void		Free() override;
};

END