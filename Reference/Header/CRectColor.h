#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRectColor : public CVIBuffer
{
private:
	explicit CRectColor();
	explicit CRectColor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectColor(const CRectColor& rhs);
	virtual ~CRectColor() override;

public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

public:
	static CRectColor*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return RECTCOLOR; }

private:
	virtual void			Free() override;
};

END

