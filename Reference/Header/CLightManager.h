#pragma once
 
#include "CBase.h"
#include "CLight.h"

BEGIN(Engine)

class ENGINE_DLL CLightManager : public CBase
{
	DECLARE_SINGLETON(CLightManager)
private:
	explicit CLightManager();
	virtual ~CLightManager() override;

public:
	HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
							const D3DLIGHT9* pLightInfo,
							_uint iIndex);

private:
	list<CLight*>			m_LightList;

private:
	void		Free() override;
};

END