#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex();
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _ulong dwCntX, 
		const _ulong dwCntZ, 
		const _ulong dwVtxItv,
		const wstring wsHeightMapPath);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex() override;

public:
	const _ulong			Get_VertexCountX() const { return m_dwCntX; }
	const _ulong			Get_VertexCountZ() const { return m_dwCntZ; }
	const _ulong			Get_VertexInterval() const { return m_dwVtxItv; }

	void					Set_VertexCountX(const _ulong dwCntX) { m_dwCntX = dwCntX; }
	void					Set_VertexCountZ(const _ulong dwCntZ) { m_dwCntZ = dwCntZ; }
	void					Set_VertexInterval(const _ulong dwVtxItv) { m_dwVtxItv = dwVtxItv; }

public:
	HRESULT					Ready_Buffer() override;
	void					Render_Buffer() override;

private:
	_ulong					m_dwCntX;
	_ulong					m_dwCntZ;
	_ulong					m_dwVtxItv;
	const wstring			m_wsHeightMapPath;

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _ulong dwCntX, 
								const _ulong dwCntZ, 
								const _ulong dwVtxItv, 
								const wstring wsHeightMapPath);
	CComponent* Clone() override;
	PROTOTYPE_COMPONENT		Get_ComponentType() override { return TERRAINTEX; }

private:
	void			Free() override;
};

END