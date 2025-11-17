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

HRESULT CRectTex::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 4;
    m_dwTriCnt = 2;
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXTEX* pVertices = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertices, NULL);

    pVertices[0].vPosition = { -1.f, 1.f, 0.f };
    pVertices[0].vTexUV = { 0.f, 0.f };
    m_vecPos.push_back(pVertices[0].vPosition);

    pVertices[1].vPosition = { 1.f, 1.f, 0.f };
    pVertices[1].vTexUV = { 1.f, 0.f};
    m_vecPos.push_back(pVertices[1].vPosition);

    pVertices[2].vPosition = { 1.f, -1.f, 0.f };
    pVertices[2].vTexUV = { 1.f, 1.f };
    m_vecPos.push_back(pVertices[2].vPosition);

    pVertices[3].vPosition = { -1.f, -1.f, 0.f };
    pVertices[3].vTexUV = { 0.f, 1.f };
    m_vecPos.push_back(pVertices[3].vPosition);

    m_pVB->Unlock();

    INDEX32* pIndices = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndices, NULL);

    pIndices[0]._0 = 0;
    pIndices[0]._1 = 1;
    pIndices[0]._2 = 2;

    pIndices[1]._0 = 0;
    pIndices[1]._1 = 2;
    pIndices[1]._2 = 3;

    m_pIB->Unlock();

    return S_OK;
}

void CRectTex::Render_Buffer()
{
    Engine::CVIBuffer::Render_Buffer();
}

CRectTex* CRectTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRectTex* pInstance = new CRectTex(pGraphicDev);

    if (FAILED(pInstance->Ready_Buffer()))
    {
        MSG_BOX("Failed CRectTex Create");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CComponent* CRectTex::Clone()
{
    return new CRectTex(*this);
}

void CRectTex::Free()
{
    Engine::CVIBuffer::Free();
}
