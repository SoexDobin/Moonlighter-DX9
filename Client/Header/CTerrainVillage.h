#pragma once
#include "CRenderObject.h"

namespace Engine
{
	class CTerrainTex;
	class CTransform;
	class CTexture;
}

class CTerrainVillage : public CRenderObject
{
private:
	explicit CTerrainVillage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainVillage(const CTerrainVillage& rhs);
	virtual ~CTerrainVillage();

	Engine::CTerrainTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	HRESULT Add_Component();
	virtual void Free();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float fTimeDelta);
	virtual void LateUpdate_GameObject(const _float fTimeDelta);
	virtual	void Render_GameObject();

	static CTerrainVillage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

