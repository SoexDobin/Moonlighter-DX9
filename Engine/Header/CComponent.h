#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() override;

public:
	virtual	PROTOTYPE_COMPONENT	Get_ComponentType() PURE;

public:
	virtual _int				Update_Component(const _float fTimeDelta) { return 0; };
	virtual void				LateUpdate_Component() { };

public:
	virtual CComponent*			Clone() PURE;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDevice;
	_bool						m_bClone;

protected:
	virtual void				Free();

#pragma region Editor
public:
	virtual void				Display_Editor(const char* pObjTag) {};
	_bool						m_bDisplayInEditor;
	TCHAR						m_szDisplayName[32];
#pragma endregion

};

END