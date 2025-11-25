#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTerrainBossTex;
    class CTexture;
}

class CTerrainBoss : public CRenderObject
{
private:
    explicit CTerrainBoss(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrainBoss(const CTerrainBoss& rhs);
    virtual ~CTerrainBoss();

    Engine::CTerrainBossTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;

    HRESULT	Add_Component();
    virtual void Free();

public:
    virtual HRESULT	Ready_GameObject() override;
    virtual	_int Update_GameObject(const _float fTimeDelta) override;
    virtual void LateUpdate_GameObject(const _float fTimeDelta) override;
    virtual	void Render_GameObject()override;

    CTerrainBossTex* Get_TerrainBuffer() { return m_pBufferCom; }

    static CTerrainBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

