#pragma once
#include "CRectColor.h"
#include "CRectTex.h"
#include "CCubeTex.h"
#include "CTerrainTex.h"
#include "CTransform.h"

#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CPrototypeManager :  public CBase
{
	DECLARE_SINGLETON(CPrototypeManager)

private:
	explicit CPrototypeManager();
	virtual ~CPrototypeManager() override;

public:
	HRESULT			Ready_Prototype(PROTOTYPE_COMPONENT pComponentTag, CComponent* pComponent);
    CComponent* Clone_Prototype(PROTOTYPE_COMPONENT pComponentTag, CGameObject* pOwner);
    CComponent* Clone_Prototype(PROTOTYPE_COMPONENT pComponentTag);

private:
	CComponent*		Find_Prototype(PROTOTYPE_COMPONENT pComponentTag);

private:
	unordered_map<PROTOTYPE_COMPONENT, CComponent*>		m_umPrototype;

private:
	virtual void Free() override;
};

END
