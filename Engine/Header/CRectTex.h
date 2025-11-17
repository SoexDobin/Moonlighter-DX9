#pragma once

#include "CVIBuffer.h"

class CRectTex : public CVIBuffer
{
private:
	explicit CRectTex();
	explicit CRectTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectTex(const CRectTex& rhs);
	virtual ~CRectTex() override;

};

