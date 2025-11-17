#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex();
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex() override;

public:
	HRESULT					Ready_Buffer() override;
	void					Render_Buffer() override;

public:
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*				Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return CUBETEX; }

private:
	virtual void			Free() override;
};

END