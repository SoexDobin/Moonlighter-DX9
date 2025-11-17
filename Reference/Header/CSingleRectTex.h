#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRectTex : public CVIBuffer
{
private:
	explicit CRectTex();
	explicit CRectTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectTex(const CRectTex& rhs);
	virtual ~CRectTex() override;

public:
	virtual	HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

public:
	static CRectTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*			Clone();
	PROTOTYPE_COMPONENT			Get_ComponentType() override { return SINGLERECTTEX; }

private:
	virtual void		Free() override;
};

END