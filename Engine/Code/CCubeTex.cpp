#include "CCubeTex.h"

CCubeTex::CCubeTex()
{

}

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

CCubeTex::CCubeTex(const CCubeTex& rhs)
	: CVIBuffer(rhs)
{
}

CCubeTex::~CCubeTex()
{
}

HRESULT CCubeTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCUBE* pVertics = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertics, NULL);

	pVertics[0].vPosition = { -1.f, 1.f, -1.f };
	pVertics[0].vTexUV = pVertics[0].vPosition;

	pVertics[1].vPosition = { 1.f, 1.f, -1.f };
	pVertics[1].vTexUV = pVertics[1].vPosition;

	pVertics[2].vPosition = { 1.f, -1.f, -1.f };
	pVertics[2].vTexUV = pVertics[2].vPosition;

	pVertics[3].vPosition = { -1.f, -1.f, -1.f };
	pVertics[3].vTexUV = pVertics[3].vPosition;

	pVertics[4].vPosition = { -1.f, 1.f, 1.f };
	pVertics[4].vTexUV = pVertics[4].vPosition;

	pVertics[5].vPosition = { 1.f, 1.f, 1.f };
	pVertics[5].vTexUV = pVertics[5].vPosition;

	pVertics[6].vPosition = { 1.f, -1.f, 1.f };
	pVertics[6].vTexUV = pVertics[6].vPosition;

	pVertics[7].vPosition = { -1.f, -1.f, 1.f };
	pVertics[7].vTexUV = pVertics[7].vPosition;

	m_pVB->Unlock();

	INDEX32* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, NULL);

	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	pIndices[8]._0 = 7; pIndices[8]._1 = 6; pIndices[8]._2 = 5;
	pIndices[9]._0 = 7;	pIndices[9]._1 = 5; pIndices[9]._2 = 4;

	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CCubeTex::Render_Buffer()
{
	Engine::CVIBuffer::Render_Buffer();
}

CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTex* pCubeTex = new CCubeTex(pGraphicDev);

	if (FAILED(pCubeTex->Ready_Buffer()))
	{
		Safe_Release(pCubeTex);
		MSG_BOX("CubeTex Create Failed");
		return nullptr;
	}

	return pCubeTex;
}

CComponent* CCubeTex::Clone()
{
	return new CCubeTex(*this);
}

void CCubeTex::Free()
{
	Engine::CVIBuffer::Free();
}
