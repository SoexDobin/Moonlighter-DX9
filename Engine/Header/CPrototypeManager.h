#pragma once
#include "CRectColor.h"
#include "CRectTex.h"
#include "CCubeTex.h"
#include "CTerrainTex.h"
#include "CTransform.h"

#include "CTexture.h"
#include "CSphereCollider.h"
#include "CRectCollider.h"

BEGIN(Engine)

class ENGINE_DLL CPrototypeManager :  public CBase
{
	DECLARE_SINGLETON(CPrototypeManager)

private:
	explicit CPrototypeManager();
	virtual ~CPrototypeManager() override;

public:
	HRESULT		Ready_Prototype(PROTOTYPE_COMPONENT eComponentTag, CComponent* pComponent);
    HRESULT		Ready_Prototype(const wstring& wsCustomComponentTag, CComponent* pComponent);

    CComponent* Clone_Prototype(PROTOTYPE_COMPONENT eComponentTag, CGameObject* pOwner);
    CComponent* Clone_Prototype(PROTOTYPE_COMPONENT eComponentTag);
    CComponent* Clone_Prototype(const wstring& wsCustomComponentTag);

private:
	CComponent*		Find_Prototype(PROTOTYPE_COMPONENT eComponentTag);
    CComponent*     Find_CustomPrototype(const wstring& wsComponentTag);

private:
	unordered_map<PROTOTYPE_COMPONENT, CComponent*>		m_umPrototype;
    unordered_map<wstring, CComponent*>		            m_umCustomPrototype;

private:
	virtual void Free() override;
};

END
