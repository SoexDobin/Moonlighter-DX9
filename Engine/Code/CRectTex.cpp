#include "CRectTex.h"

CRectTex::CRectTex()
{
}

CRectTex::CRectTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRectTex::CRectTex(const CRectTex& rhs)
	: CVIBuffer(rhs)
{
}

CRectTex::~CRectTex()
{
}
