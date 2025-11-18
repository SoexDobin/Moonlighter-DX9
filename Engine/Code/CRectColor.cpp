#include "CRectColor.h"

CRectColor::CRectColor()
{
}

CRectColor::CRectColor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRectColor::CRectColor(const CRectColor& rhs)
	: CVIBuffer(rhs)
{
}

CRectColor::~CRectColor()
{
}

HRESULT CRectColor::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* pVertices = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = { -1.f, 1.f, 0.f };
	pVertices[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertices[1].vPosition = { 1.f, 1.f, 0.f };
	pVertices[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertices[2].vPosition = { 1.f, -1.f, 0.f };
	pVertices[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertices[3].vPosition = { -1.f, -1.f, 0.f };
	pVertices[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndices = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRectColor::Render_Buffer()
{
	Engine::CVIBuffer::Render_Buffer();
}

CRectColor* CRectColor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRectColor* pRectCol = new CRectColor(pGraphicDev);

	if (FAILED(pRectCol->Ready_Buffer()))
	{
		Safe_Release(pRectCol);
		MSG_BOX("RectColor Create Failed");
		return nullptr;
	}

	return pRectCol;
}

CComponent* CRectColor::Clone()
{
	return new CRectColor(*this);
}

void CRectColor::Free()
{
	Engine::CVIBuffer::Free();
}
