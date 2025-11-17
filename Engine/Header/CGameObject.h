#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() override;

public:
	CComponent*		Get_Component(COMPONENTID eID, const _tchar* pComponentTag);

private:
	CComponent*		Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float fTimeDelta);
	virtual		void		Render_GameObject();

protected:
	LPDIRECT3DDEVICE9							m_pGraphicDevice;
	unordered_map<const _tchar*, CComponent*>	m_umComponent[ID_END];

protected:
	virtual void Free() override;
};

END