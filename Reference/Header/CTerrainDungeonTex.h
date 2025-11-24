#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainDungeonTex : public CVIBuffer
{
private:
    explicit CTerrainDungeonTex();
    explicit CTerrainDungeonTex(LPDIRECT3DDEVICE9 pGraphicDev,
        const _ulong dwCntX,
        const _ulong dwCntZ,
        const _ulong dwVtxItv,
        const wstring& wsHeightMapPath);
    explicit CTerrainDungeonTex(const CTerrainDungeonTex& rhs);
    virtual ~CTerrainDungeonTex() override;

public:
    const _ulong			Get_VertexCountX() const { return m_dwCntX; }
    const _ulong			Get_VertexCountZ() const { return m_dwCntZ; }
    const _ulong			Get_VertexInterval() const { return m_dwVtxItv; }
    const wstring           Get_HeightMap() const { return m_wsHeightMapPath; }

    void					Set_VertexCountX(const _ulong dwCntX) { m_dwCntX = dwCntX; }
    void					Set_VertexCountZ(const _ulong dwCntZ) { m_dwCntZ = dwCntZ; }
    void					Set_VertexInterval(const _ulong dwVtxItv) { m_dwVtxItv = dwVtxItv; }
    void                    Set_HeightMap(const wstring& dwHeightMap) { m_wsHeightMapPath = dwHeightMap; }

public:
    HRESULT					Ready_Buffer() override;
    void					Render_Buffer() override;

private:
    _ulong					m_dwCntX;
    _ulong					m_dwCntZ;
    _ulong					m_dwVtxItv;
    wstring			m_wsHeightMapPath;
    PROTOTYPE_COMPONENT m_eProto;

public:
    static CTerrainDungeonTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _ulong dwCntX,
        const _ulong dwCntZ,
        const _ulong dwVtxItv,
        const wstring& wsHeightMapPath);
    CComponent* Clone() override;
    PROTOTYPE_COMPONENT		Get_ComponentType() override { return TERRAINDUNGEONTEX; }

private:
    void			Free() override;
};

END
