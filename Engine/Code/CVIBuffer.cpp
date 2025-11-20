#include "CVIBuffer.h"
#include "CRenderer.h"

CVIBuffer::CVIBuffer() : m_pVB(nullptr), m_pIB(nullptr)
, m_dwTriCnt(0), m_dwVtxCnt(0), m_dwVtxSize(0), m_dwFVF(0)
, m_dwIdxSize(0), m_IdxFmt(D3DFMT_INDEX32)
{
    strcpy_s(m_szDisplayName, "Buffer");
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), 
    m_pVB(nullptr), m_pIB(nullptr)
    , m_dwTriCnt(0), m_dwVtxCnt(0), m_dwVtxSize(0), m_dwFVF(0)
    , m_dwIdxSize(0), m_IdxFmt(D3DFMT_INDEX32)
{
    strcpy_s(m_szDisplayName, "Buffer");
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
    : CComponent(rhs), 
    m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB), 
    m_dwTriCnt(rhs.m_dwTriCnt), m_dwVtxCnt(rhs.m_dwVtxCnt), m_dwVtxSize(rhs.m_dwVtxSize),
    m_dwFVF(rhs.m_dwFVF), m_dwIdxSize(rhs.m_dwIdxSize), m_IdxFmt(rhs.m_IdxFmt)
{
    m_pVB->AddRef();
    m_pIB->AddRef();
    strcpy_s(m_szDisplayName, "Buffer");
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
    if (FAILED(m_pGraphicDevice->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,
                                                    0, m_dwFVF,
                                                    D3DPOOL_MANAGED, 
                                                    &m_pVB,     
                                                    NULL)))     
                                                    return E_FAIL;

    if (FAILED(m_pGraphicDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
                                                   0, m_IdxFmt,
                                                   D3DPOOL_MANAGED,
                                                   &m_pIB,
                                                   NULL)))
                                                   return E_FAIL;

    return S_OK;
}

void CVIBuffer::Render_Buffer()
{
    m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
    m_pGraphicDevice->SetFVF(m_dwFVF);

    m_pGraphicDevice->SetIndices(m_pIB);
    m_pGraphicDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

    CRenderer::GetInstance()->Inc_DrawCalls();
}

void CVIBuffer::Free()
{
    Safe_Release(m_pVB);
    Safe_Release(m_pIB);

    CComponent::Free();
}

CComponent* CVIBuffer::Clone()
{
    return nullptr;
}

void CVIBuffer::Display_Editor(const char* pObjTag)
{

        ImGui::Text("VTX count : %lu", m_dwVtxCnt);

        TCHAR szBuff[32];
        switch (m_dwFVF)
        {
        case FVF_COL:
            wcscpy_s(szBuff, L"VTX COL");
            break;
        case FVF_TEX:
            wcscpy_s(szBuff, L"VTX TEX");
            break;
        case FVF_CUBE:
            wcscpy_s(szBuff, L"VTX CUBE");
            break;

        }
        ImGui::Text("FVF : %ls", szBuff);
}
