#pragma once
#include "CRenderObject.h"

namespace Engine
{
    class CTerrainTex;
    class CTexture;
}

class CTerrainVillage : public CRenderObject
{
private:
    explicit CTerrainVillage(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrainVillage(const CTerrainVillage& rhs);
    virtual ~CTerrainVillage();

    Engine::CTerrainTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;

    HRESULT	Add_Component();
    virtual void Free();

public:
    virtual HRESULT	Ready_GameObject() override;
    virtual	_int Update_GameObject(const _float fTimeDelta) override;
    virtual void LateUpdate_GameObject(const _float fTimeDelta) override;
    virtual	void Render_GameObject()override;

    CTerrainTex* Get_TerrainBuffer() { return m_pBufferCom; }
    //void Set_BufferCom(Engine::CTerrainTex* pTex)
    //{
    //    if (m_pBufferCom)
    //        Safe_Release(m_pBufferCom);

    //    m_pBufferCom = pTex;
    //}

    static CTerrainVillage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

