#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSingleRectTex : public CVIBuffer
{
private:
	explicit CSingleRectTex();
	explicit CSingleRectTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSingleRectTex(const CSingleRectTex& rhs);
	virtual ~CSingleRectTex() override;

public:
	virtual	HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

public:
	static CSingleRectTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*			Clone();
	PROTOTYPE_COMPONENT			Get_ComponentType() override { return SINGLERECTTEX; }

private:
	virtual void		Free() override;
};

END