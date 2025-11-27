#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTerrainDungeonTex;
    class CTexture;
}

class CTerrainDungeonNormal : public CRenderObject
{
private:
    explicit CTerrainDungeonNormal(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrainDungeonNormal(const CTerrainDungeonNormal& rhs);
    virtual ~CTerrainDungeonNormal();

    Engine::CTerrainDungeonTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;

    HRESULT	Add_Component();
    virtual void Free();

public:
    virtual HRESULT	Ready_GameObject() override;
    virtual	_int Update_GameObject(const _float fTimeDelta) override;
    virtual void LateUpdate_GameObject(const _float fTimeDelta) override;
    virtual	void Render_GameObject()override;

    CTerrainDungeonTex* Get_TerrainBuffer() { return m_pBufferCom; }

    static CTerrainDungeonNormal* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

