#include "CTerrainBossTex.h"

CTerrainBossTex::CTerrainBossTex()
    : m_dwCntX(0), m_dwCntZ(0), m_dwVtxItv(0), m_wsHeightMapPath(L"")
{
}

CTerrainBossTex::CTerrainBossTex(LPDIRECT3DDEVICE9 pGraphicDev,
    const _ulong dwCntX,
    const _ulong dwCntZ,
    const _ulong dwVtxItv,
    const wstring& wsHeightMapPath)
    : CVIBuffer(pGraphicDev),
    m_dwCntX(dwCntX), m_dwCntZ(dwCntZ), m_dwVtxItv(dwVtxItv), m_wsHeightMapPath(wsHeightMapPath)
{
}

CTerrainBossTex::CTerrainBossTex(const CTerrainBossTex& rhs)
    : CVIBuffer(rhs),
    m_dwCntX(rhs.m_dwCntX), m_dwCntZ(rhs.m_dwCntZ), m_dwVtxItv(rhs.m_dwVtxItv), m_wsHeightMapPath(rhs.m_wsHeightMapPath)
{

}

CTerrainBossTex::~CTerrainBossTex()
{
}

HRESULT CTerrainBossTex::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = m_dwCntX * m_dwCntZ;
    m_dwTriCnt = (m_dwCntX - 1) * (m_dwCntZ - 1) * 2;
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(Engine::CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    _ulong* pPixel(nullptr);
    if (!m_wsHeightMapPath.empty())
    {
        _ulong dwByte(0);
        BITMAPFILEHEADER tFileHeader;
        BITMAPINFOHEADER tInfoHeader;
        HANDLE hFile = CreateFile(m_wsHeightMapPath.c_str(),
            GENERIC_READ,
            NULL, NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE) return E_FAIL;

        if (!ReadFile(hFile, &tFileHeader, sizeof(BITMAPFILEHEADER), &dwByte, NULL))
            return E_FAIL;
        if (!ReadFile(hFile, &tInfoHeader, sizeof(BITMAPINFOHEADER), &dwByte, NULL))
            return E_FAIL;

        pPixel = new _ulong[tInfoHeader.biWidth * tInfoHeader.biHeight];
        if (!ReadFile(hFile, pPixel, sizeof(_ulong) * tInfoHeader.biWidth * tInfoHeader.biHeight, &dwByte, NULL))
            return E_FAIL;
    }

    VTXTEX* pVertices = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertices, NULL);

    _ulong dwIndex(0);
    _float fHeight(0.f);
    for (_ulong i = 0; i < m_dwCntZ; ++i)
    {
        for (_ulong j = 0; j < m_dwCntX; ++j)
        {
            dwIndex = i * m_dwCntX + j;
            if (pPixel != nullptr)
                fHeight = (_float)(pPixel[dwIndex] & 0x000000ff) / 20.f;

            pVertices[dwIndex].vPosition =
            { (_float)(j * m_dwVtxItv),
              fHeight,
              (_float)(i * m_dwVtxItv) };
            pVertices[dwIndex].vTexUV =
            { (_float)j / _float(m_dwCntX - 1), (_float)i / _float(m_dwCntZ - 1) };
        }
    }

    m_pVB->Unlock();
    Safe_Delete_Array(pPixel);

    INDEX32* pIndices = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndices, NULL);

    dwIndex = 0;
    _ulong dwTriCnt(0);
    for (_ulong i = 0; i < m_dwCntZ - 1; ++i)
    {
        for (_ulong j = 0; j < m_dwCntX - 1; ++j)
        {
            _ulong v0 = i * m_dwCntX + j;
            _ulong v1 = v0 + 1;
            _ulong v2 = v0 + m_dwCntX;
            _ulong v3 = v2 + 1;

            pIndices[dwTriCnt]._0 = v0;
            pIndices[dwTriCnt]._1 = v2;
            pIndices[dwTriCnt]._2 = v1;
            ++dwTriCnt;

            pIndices[dwTriCnt]._0 = v1;
            pIndices[dwTriCnt]._1 = v2;
            pIndices[dwTriCnt]._2 = v3;
            ++dwTriCnt;
        }
    }

    m_pIB->Unlock();

    return S_OK;
}

void CTerrainBossTex::Render_Buffer()
{
    Engine::CVIBuffer::Render_Buffer();
}

CTerrainBossTex* CTerrainBossTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
    const _ulong dwCntX,
    const _ulong dwCntZ,
    const _ulong dwVtxItv,
    const wstring& wsHeightMapPath)
{
    CTerrainBossTex* pInstance = new CTerrainBossTex(pGraphicDev, dwCntX, dwCntZ, dwVtxItv, wsHeightMapPath);

    if (FAILED(pInstance->Ready_Buffer()))
    {
        MSG_BOX("CTerrainBossTex Create Failed");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

CComponent* CTerrainBossTex::Clone()
{
    return new CTerrainBossTex(*this);
}

void CTerrainBossTex::Free()
{
    Engine::CVIBuffer::Free();
}
