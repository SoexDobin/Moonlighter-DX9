#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() override;

public:
    void                        Set_Owner(CGameObject* pOwner);
    CGameObject*                Get_Owner() const       { return m_pOwner; }
    CTransform*                 Get_Transform() const   { return m_pTrans; }
	virtual	PROTOTYPE_COMPONENT	Get_ComponentType() PURE;

public:
	virtual _int				Update_Component(const _float fTimeDelta) { return 0; };
	virtual void				LateUpdate_Component() { };

public:
	virtual CComponent*			Clone() PURE;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDevice;
	_bool						m_bClone;

    CGameObject*                m_pOwner;
    CTransform*                 m_pTrans;

protected:
	virtual void				Free();

#pragma region Editor
public:
	virtual void				Display_Editor(const char* pObjTag) {};
	_bool						m_bDisplayInEditor;
	char					    m_szDisplayName[32];
#pragma endregion

};

END
